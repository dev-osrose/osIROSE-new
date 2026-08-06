#include "gtest/gtest.h"

#include "config.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <vector>

// Core::Config is the single source of truth for every port, credential and
// path the four servers use, and it is a Meyers singleton keyed on the filename
// passed to the *first* getInstance() call. That means:
//
//   * exactly one Config exists per process, so this whole file gets one shot
//     at loading one file -- hence the global environment below rather than a
//     per-test fixture;
//   * the values asserted here have to differ from the compiled-in defaults in
//     config.h, or the test cannot tell parsing from not-parsing. The previous
//     version of this file asserted the defaults against themselves and passed
//     without ever reading a file.
//
// Config's constructor also *writes* a defaults file when the named one is
// missing (config.cpp:33-35), so everything here runs in a scratch directory
// that is created before the first getInstance() and removed afterwards.

namespace {

// Deliberately none of the config.h defaults.
constexpr const char* kConfigJson = R"({
  "database": {
    "host": "db.example.invalid",
    "database": "rosetest",
    "user": "tester",
    "password": "s3cr3t",
    "port": 3307
  },
  "serverData": {
    "id": 7,
    "externalIp": "10.0.0.1",
    "listenIp": "0.0.0.0",
    "iscListenIp": "10.0.0.2",
    "autoConfigureUrl": "http://example.invalid/raw",
    "core_dump_path": "/var/crash",
    "crash_report_url": "http://example.invalid/crash",
    "parentId": 3,
    "maxConnections": 500,
    "useThreads": false,
    "autoConfigureAddress": true,
    "maxThreads": 64,
    "accessLevel": 9,
    "mode": 2
  },
  "loginServer": {
    "createAccountOnFail": true,
    "clientPort": 39000,
    "iscPort": 39010,
    "accessLevel": 5,
    "logLevel": 1
  },
  "charServer": {
    "worldName": "TestWorld",
    "loginIp": "10.0.0.3",
    "loginUser": "chartest",
    "loginPassword": "charhash",
    "clientPort": 39100,
    "iscPort": 39110,
    "healthPort": 40100,
    "instantCharDelete": false,
    "accessLevel": 4,
    "logLevel": 3
  },
  "mapServer": {
    "channelName": "TestChannel",
    "charIp": "10.0.0.4",
    "charUser": "maptest",
    "charPassword": "maphash",
    "clientPort": 39200,
    "iscPort": 39210,
    "healthPort": 40200,
    "accessLevel": 6,
    "mapId": [11, 22, 33],
    "luaScript": "scripts/test_root.lua",
    "logLevel": 4
  },
  "nodeServer": {
    "loginIp": "10.0.0.5",
    "loginPort": 39001,
    "healthPort": 40001,
    "logLevel": 5
  }
}
)";

std::filesystem::path g_scratch_dir;
std::filesystem::path g_original_dir;

// Runs before any test, which is the only place the config file can be put in
// position: the first getInstance() call anywhere in the process wins.
class ConfigEnvironment : public ::testing::Environment {
 public:
  void SetUp() override {
    g_original_dir = std::filesystem::current_path();
    // Unique per run so concurrent ctest jobs cannot collide. std::random_device
    // rather than getpid() because this has to build on MSVC too.
    g_scratch_dir = std::filesystem::temp_directory_path() /
                    ("osirose-config-test-" + std::to_string(std::random_device{}()));
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
    ::testing::AddGlobalTestEnvironment(new ConfigEnvironment);

}  // namespace

TEST(Config, ParsesTheDatabaseSection) {
  auto& db = Core::Config::getInstance().database();

  EXPECT_EQ("db.example.invalid", db.host);
  EXPECT_EQ("rosetest", db.database);
  EXPECT_EQ("tester", db.user);
  EXPECT_EQ("s3cr3t", db.password);
  EXPECT_EQ(3307, db.port);

  // The compiled defaults, spelled out so it is obvious the values above are
  // not them. This is the assertion the old test was missing.
  EXPECT_NE("127.0.0.1", db.host);
  EXPECT_NE("osirose", db.database);
  EXPECT_NE("root", db.user);
  EXPECT_NE(3306, db.port);
}

TEST(Config, ParsesTheServerDataSection) {
  auto& sd = Core::Config::getInstance().serverData();

  EXPECT_EQ(7u, sd.id);
  EXPECT_EQ("10.0.0.1", sd.externalIp);
  EXPECT_EQ("0.0.0.0", sd.listenIp);
  EXPECT_EQ("10.0.0.2", sd.iscListenIp);
  EXPECT_EQ("http://example.invalid/raw", sd.autoConfigureUrl);
  EXPECT_EQ("/var/crash", sd.core_dump_path);
  EXPECT_EQ("http://example.invalid/crash", sd.crash_report_url);
  EXPECT_EQ(3u, sd.parentId);
  EXPECT_EQ(500u, sd.maxConnections);
  EXPECT_EQ(64u, sd.maxThreads);
  EXPECT_EQ(9, sd.accessLevel);
  EXPECT_EQ(2, sd.mode);
}

// Booleans are the easiest field to get wrong in a hand-written deserialiser,
// and both of these default to the opposite of what the file says.
TEST(Config, ParsesBooleansInBothDirections) {
  auto& sd = Core::Config::getInstance().serverData();

  EXPECT_FALSE(sd.useThreads) << "default is true, file says false";
  EXPECT_TRUE(sd.autoConfigureAddress) << "default is false, file says true";
  EXPECT_TRUE(Core::Config::getInstance().loginServer().createAccountOnFail);
  EXPECT_FALSE(Core::Config::getInstance().charServer().instantCharDelete);
}

TEST(Config, ParsesTheLoginServerSection) {
  auto& login = Core::Config::getInstance().loginServer();

  EXPECT_EQ(39000, login.clientPort);
  EXPECT_EQ(39010, login.iscPort);
  EXPECT_EQ(5, login.accessLevel);
  EXPECT_EQ(1, login.logLevel);
}

// Partial sections are the normal case, not an edge case: the shipped
// config/server.json omits healthPort from all four server sections, so every
// deployment is already relying on the compiled default surviving a parse. The
// test file above omits it from loginServer only, and asserts it elsewhere.
TEST(Config, KeysAbsentFromTheFileKeepTheirCompiledDefault) {
  auto& config = Core::Config::getInstance();

  EXPECT_EQ(30000, config.loginServer().healthPort) << "default from config.h";

  // Sibling keys in the same partial section still parse.
  EXPECT_EQ(39000, config.loginServer().clientPort);
  EXPECT_TRUE(config.loginServer().createAccountOnFail);

  // And a partial section does not disturb the fully specified ones.
  EXPECT_EQ(40100, config.charServer().healthPort);
  EXPECT_EQ(40200, config.mapServer().healthPort);
  EXPECT_EQ(40001, config.nodeServer().healthPort);
}

TEST(Config, ParsesTheCharServerSection) {
  auto& ch = Core::Config::getInstance().charServer();

  EXPECT_EQ("TestWorld", ch.worldName);
  EXPECT_EQ("10.0.0.3", ch.loginIp);
  EXPECT_EQ("chartest", ch.loginUser);
  EXPECT_EQ("charhash", ch.loginPassword);
  EXPECT_EQ(39100, ch.clientPort);
  EXPECT_EQ(39110, ch.iscPort);
  EXPECT_EQ(40100, ch.healthPort);
  EXPECT_EQ(4, ch.accessLevel);
  EXPECT_EQ(3, ch.logLevel);
}

TEST(Config, ParsesTheMapServerSection) {
  auto& map = Core::Config::getInstance().mapServer();

  EXPECT_EQ("TestChannel", map.channelName);
  EXPECT_EQ("10.0.0.4", map.charIp);
  EXPECT_EQ("maptest", map.charUser);
  EXPECT_EQ("maphash", map.charPassword);
  EXPECT_EQ(39200, map.clientPort);
  EXPECT_EQ(39210, map.iscPort);
  EXPECT_EQ(40200, map.healthPort);
  EXPECT_EQ(6, map.accessLevel);
  EXPECT_EQ("scripts/test_root.lua", map.luaScript);
  EXPECT_EQ(4, map.logLevel);
}

// mapId is the only container in the whole schema. The default is a 35-element
// list, so a deserialiser that appended instead of replacing would show up here.
TEST(Config, ParsesTheMapIdListAndReplacesTheDefault) {
  auto& map = Core::Config::getInstance().mapServer();

  EXPECT_EQ((std::vector<uint16_t>{11, 22, 33}), map.mapId);
}

TEST(Config, ParsesTheNodeServerSection) {
  auto& node = Core::Config::getInstance().nodeServer();

  EXPECT_EQ("10.0.0.5", node.loginIp);
  EXPECT_EQ(39001, node.loginPort);
  EXPECT_EQ(40001, node.healthPort);
  EXPECT_EQ(5, node.logLevel);
}

// The documented footgun, asserted rather than left to the reader: the filename
// only matters on the very first call. Every later call gets the instance built
// from whatever the first caller asked for -- and does not even touch the disk,
// so a server that calls getInstance("map.json") after something else already
// called getInstance() silently runs on the other server's configuration.
TEST(Config, IgnoresTheFilenameAfterTheFirstCall) {
  auto& first = Core::Config::getInstance();
  auto& second = Core::Config::getInstance("some-other-config.json");

  EXPECT_EQ(&first, &second) << "a second getInstance built a new Config";
  EXPECT_EQ("db.example.invalid", second.database().host)
      << "the second filename was actually loaded";
  EXPECT_FALSE(std::filesystem::exists("some-other-config.json"))
      << "the second call went to disk";
}

// The accessors hand out non-const references into singleton state, so any
// caller can rewrite the global configuration at runtime. Documented here
// because it is invisible at every call site.
TEST(Config, AccessorsExposeMutableSingletonState) {
  auto& db = Core::Config::getInstance().database();
  const std::string original = db.host;

  db.host = "mutated.invalid";
  EXPECT_EQ("mutated.invalid", Core::Config::getInstance().database().host);

  db.host = original;  // other tests read this
  EXPECT_EQ(original, Core::Config::getInstance().database().host);
}

// Nothing may be written into the directory the tests run from. The old version
// of this file left a server.json behind wherever ctest happened to be.
TEST(Config, WritesNoUnexpectedFiles) {
  std::vector<std::string> found;
  for (const auto& entry : std::filesystem::directory_iterator(".")) {
    found.push_back(entry.path().filename().string());
  }

  EXPECT_EQ((std::vector<std::string>{"server.json"}), found);
}
