#include "gtest/gtest.h"

#include "config.h"
#include "rose_ssl_config.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <string>

// rose_ssl_config.h is the whole of the bridge between the [ssl] block in
// server.json and the TLS setters on the sockets. Nothing else in this repo
// touches Core::SslServerConfig / Core::SslClientConfig -- the four main()s and
// map_manager.cpp only ever call ApplySslServerConfig / ApplySslClientConfig and
// branch on the bool -- so if the mapping here is wrong, every server starts
// with the wrong TLS material and no call site shows it.
//
// The helpers are templates, so the socket can be a local stub: this file makes
// no network calls, needs no certificates, and links only Core.
//
// USE_SSL comes through Core -> osirose_common -> osIROSE-common-core, which
// defines it PUBLIC when the build is configured with -DENABLE_SSL=ON. Both
// flavours are tested below; the plaintext one is a real code path, not an
// absence of one.

namespace {

// Config is a Meyers singleton keyed on the filename handed to the *first*
// getInstance() call, so as in test_config.cpp the file has to be in place
// before any test runs -- hence a global environment rather than a fixture.
// Its constructor also writes a defaults file when the named one is missing,
// so this runs in a scratch directory.
constexpr const char* kConfigJson = R"({
  "ssl": {
    "certificateChainFile": "/etc/rose/chain.pem",
    "privateKeyFile": "/etc/rose/key.pem",
    "privateKeyPassword": "keypass",
    "dhParamsFile": "/etc/rose/dh.pem",
    "clientCaFile": "/etc/rose/clients.pem",
    "requireClientCert": false,
    "handshakeTimeoutSeconds": 42,
    "verifyPeer": true,
    "caFile": "/etc/rose/ca.pem",
    "caPath": "/etc/rose/ca.d",
    "sniHostname": "",
    "cipherList": "ECDHE-RSA-AES256-GCM-SHA384"
  }
}
)";

std::filesystem::path g_scratch_dir;
std::filesystem::path g_original_dir;

class SslConfigEnvironment : public ::testing::Environment {
 public:
  void SetUp() override {
    g_original_dir = std::filesystem::current_path();
    g_scratch_dir = std::filesystem::temp_directory_path() /
                    ("osirose-ssl-config-test-" + std::to_string(std::random_device{}()));
    std::filesystem::remove_all(g_scratch_dir);
    std::filesystem::create_directories(g_scratch_dir);
    std::filesystem::current_path(g_scratch_dir);

    std::ofstream out("server.json");
    out << kConfigJson;
    out.close();
    ASSERT_TRUE(out.good()) << "could not write the test config";
  }

  void TearDown() override {
    std::filesystem::current_path(g_original_dir);
    std::filesystem::remove_all(g_scratch_dir);
  }
};

const auto* const kEnvironment =
    ::testing::AddGlobalTestEnvironment(new SslConfigEnvironment);

// Stands in for CRoseSocket / CNetwork_Asio. ApplySsl*Config is a template, so
// no INetwork inheritance is needed -- and deliberately so: the point is to
// observe exactly what the helper hands the socket, and whether it calls it at
// all.
struct StubSocket {
  bool server_result = true;
  bool client_result = true;
  int server_calls = 0;
  int client_calls = 0;
#ifdef USE_SSL
  Core::SslServerConfig last_server;
  Core::SslClientConfig last_client;

  bool enable_ssl_server(const Core::SslServerConfig& _cfg) {
    ++server_calls;
    last_server = _cfg;
    return server_result;
  }

  bool enable_ssl_client(const Core::SslClientConfig& _cfg) {
    ++client_calls;
    last_client = _cfg;
    return client_result;
  }
#endif
};

// Several tests need [ssl] values other than the ones in the file above.
// Config's accessors hand out mutable references into singleton state (see
// test_config.cpp's AccessorsExposeMutableSingletonState), so the fixture
// snapshots the section and puts it back afterwards rather than leaking the
// change into whichever test runs next.
class SslConfigTest : public ::testing::Test {
 protected:
  void SetUp() override { saved_ = Core::Config::getInstance().ssl(); }
  void TearDown() override { Core::Config::getInstance().ssl() = saved_; }

  static Core::Config& config() { return Core::Config::getInstance(); }

  Core::Config::Ssl saved_;
};

}  // namespace

#ifdef USE_SSL

TEST_F(SslConfigTest, MakeSslServerConfigMapsEveryField) {
  config().ssl().requireClientCert = true;

  const auto cfg = RoseCommon::MakeSslServerConfig(config());

  EXPECT_EQ("/etc/rose/chain.pem", cfg.certificate_chain_file);
  EXPECT_EQ("/etc/rose/key.pem", cfg.private_key_file);
  EXPECT_EQ("keypass", cfg.private_key_password);
  EXPECT_EQ("/etc/rose/dh.pem", cfg.dh_params_file);
  EXPECT_EQ("/etc/rose/clients.pem", cfg.client_ca_file);
  EXPECT_EQ("ECDHE-RSA-AES256-GCM-SHA384", cfg.cipher_list);
  EXPECT_TRUE(cfg.require_client_cert);
  EXPECT_EQ(42u, cfg.handshake_timeout_seconds);
}

// The two names differ between the config struct and the asio-facing one
// (caFile/ca_file, verifyPeer/verify_peer), which is exactly the kind of
// hand-written mapping that goes wrong silently.
TEST_F(SslConfigTest, MakeSslClientConfigMapsTheVerificationFields) {
  const auto cfg = RoseCommon::MakeSslClientConfig(config(), "peer.example.invalid");

  EXPECT_TRUE(cfg.verify_peer);
  EXPECT_EQ("/etc/rose/ca.pem", cfg.ca_file);
  EXPECT_EQ("/etc/rose/ca.d", cfg.ca_path);
  EXPECT_EQ("ECDHE-RSA-AES256-GCM-SHA384", cfg.cipher_list);
}

// verify_peer is the one field where the wrong answer is silently insecure
// rather than loudly broken: the connection still establishes, it just no
// longer authenticates the peer.
TEST_F(SslConfigTest, VerifyPeerIsCarriedThroughInBothDirections) {
  EXPECT_TRUE(RoseCommon::MakeSslClientConfig(config(), "host").verify_peer);

  config().ssl().verifyPeer = false;
  EXPECT_FALSE(RoseCommon::MakeSslClientConfig(config(), "host").verify_peer);
}

// This is the map_manager.cpp / nodeclient.cpp path: with no ssl.sniHostname
// configured, the host actually being dialled is what the peer's certificate
// gets matched against.
TEST_F(SslConfigTest, SniFallsBackToTheDialledHost) {
  ASSERT_TRUE(config().ssl().sniHostname.empty()) << "precondition from the test config";

  const auto cfg = RoseCommon::MakeSslClientConfig(config(), "char.example.invalid");

  EXPECT_EQ("char.example.invalid", cfg.sni_hostname);
}

// And the documented precedence when a deployment does set one -- a shared
// certificate whose name matches none of the per-service hostnames.
TEST_F(SslConfigTest, AnExplicitSniHostnameOverridesTheDialledHost) {
  config().ssl().sniHostname = "isc.example.invalid";

  const auto cfg = RoseCommon::MakeSslClientConfig(config(), "char.example.invalid");

  EXPECT_EQ("isc.example.invalid", cfg.sni_hostname);
}

// Neither the config file nor the call sites say so: requireClientCert is a
// *server*-side flag, and MakeSslClientConfig reuses it to mean "and present
// our own certificate when a peer asks for one" (mTLS between servers). Pinned
// here because that coupling is invisible everywhere else.
TEST_F(SslConfigTest, ClientCertificateIsSentOnlyWhenRequireClientCertIsSet) {
  ASSERT_FALSE(config().ssl().requireClientCert) << "precondition from the test config";

  auto cfg = RoseCommon::MakeSslClientConfig(config(), "host");
  EXPECT_EQ("", cfg.certificate_chain_file);
  EXPECT_EQ("", cfg.private_key_file);
  EXPECT_EQ("", cfg.private_key_password);

  config().ssl().requireClientCert = true;

  cfg = RoseCommon::MakeSslClientConfig(config(), "host");
  EXPECT_EQ("/etc/rose/chain.pem", cfg.certificate_chain_file);
  EXPECT_EQ("/etc/rose/key.pem", cfg.private_key_file);
  EXPECT_EQ("keypass", cfg.private_key_password);
}

// What every main() branches on before listen(). A false here has to reach the
// caller: the servers treat it as fatal, and the alternative is a listener that
// starts without the TLS material it was configured with.
TEST_F(SslConfigTest, ApplySslServerConfigReturnsFalseWhenTheSocketRejectsIt) {
  StubSocket socket;
  socket.server_result = false;

  EXPECT_FALSE(RoseCommon::ApplySslServerConfig(socket, config(), "test listener"));
  EXPECT_EQ(1, socket.server_calls);
}

TEST_F(SslConfigTest, ApplySslServerConfigPassesTheMappedConfigThrough) {
  StubSocket socket;

  EXPECT_TRUE(RoseCommon::ApplySslServerConfig(socket, config(), "test listener"));
  ASSERT_EQ(1, socket.server_calls);

  const auto expected = RoseCommon::MakeSslServerConfig(config());
  EXPECT_EQ(expected.certificate_chain_file, socket.last_server.certificate_chain_file);
  EXPECT_EQ(expected.private_key_file, socket.last_server.private_key_file);
  EXPECT_EQ(expected.private_key_password, socket.last_server.private_key_password);
  EXPECT_EQ(expected.dh_params_file, socket.last_server.dh_params_file);
  EXPECT_EQ(expected.client_ca_file, socket.last_server.client_ca_file);
  EXPECT_EQ(expected.cipher_list, socket.last_server.cipher_list);
  EXPECT_EQ(expected.require_client_cert, socket.last_server.require_client_cert);
  EXPECT_EQ(expected.handshake_timeout_seconds, socket.last_server.handshake_timeout_seconds);
}

TEST_F(SslConfigTest, ApplySslClientConfigReturnsFalseWhenTheSocketRejectsIt) {
  StubSocket socket;
  socket.client_result = false;

  EXPECT_FALSE(
      RoseCommon::ApplySslClientConfig(socket, config(), "host.invalid", "test connection"));
  EXPECT_EQ(1, socket.client_calls);
}

TEST_F(SslConfigTest, ApplySslClientConfigPassesTheMappedConfigThrough) {
  StubSocket socket;

  EXPECT_TRUE(
      RoseCommon::ApplySslClientConfig(socket, config(), "host.invalid", "test connection"));
  ASSERT_EQ(1, socket.client_calls);

  EXPECT_TRUE(socket.last_client.verify_peer);
  EXPECT_EQ("/etc/rose/ca.pem", socket.last_client.ca_file);
  EXPECT_EQ("/etc/rose/ca.d", socket.last_client.ca_path);
  EXPECT_EQ("ECDHE-RSA-AES256-GCM-SHA384", socket.last_client.cipher_list);
  EXPECT_EQ("host.invalid", socket.last_client.sni_hostname)
      << "the dialled host has to survive the trip through Apply*";
}

#else  // !USE_SSL

// The plaintext build is a real code path with its own contract: the helpers
// succeed unconditionally so the call sites stay free of #ifdefs. What matters
// is that they compile *out* rather than half-configuring something -- a
// no-op that still reached the socket would be worse than either branch.
TEST_F(SslConfigTest, TheHelpersAreNoOpsThatSucceedWithoutTouchingTheSocket) {
  StubSocket socket;

  EXPECT_TRUE(RoseCommon::ApplySslServerConfig(socket, config(), "test listener"));
  EXPECT_TRUE(
      RoseCommon::ApplySslClientConfig(socket, config(), "host.invalid", "test connection"));

  EXPECT_EQ(0, socket.server_calls);
  EXPECT_EQ(0, socket.client_calls);
}

// The [ssl] section still parses in this build -- Core::Config::Ssl is not
// guarded by USE_SSL -- it is simply never consulted. Asserted so that turning
// ENABLE_SSL on cannot be the first time anyone finds out the section is
// unreadable.
TEST_F(SslConfigTest, TheSslSectionIsStillParsedInAPlaintextBuild) {
  EXPECT_EQ("/etc/rose/chain.pem", config().ssl().certificateChainFile);
  EXPECT_TRUE(config().ssl().verifyPeer);
}

#endif  // USE_SSL
