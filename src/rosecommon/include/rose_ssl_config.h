// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file rose_ssl_config.h
 *
 * Bridges the deployment's [ssl] configuration section onto the TLS setters on
 * Core::INetwork.
 *
 * Everything here compiles to a no-op returning true when USE_SSL is off, so
 * call sites need no #ifdef of their own.
 */
#ifndef _ROSE_SSL_CONFIG_H_
#define _ROSE_SSL_CONFIG_H_

#include <string>

#include "config.h"
#include "inetwork.h"  // Core::SslServerConfig / Core::SslClientConfig
#include "logconsole.h"

namespace RoseCommon {

#ifdef USE_SSL

/// Build the listening-socket TLS configuration from the [ssl] config section.
inline Core::SslServerConfig MakeSslServerConfig(Core::Config& _config) {
  const auto& ssl = _config.ssl();
  Core::SslServerConfig cfg;
  cfg.certificate_chain_file    = ssl.certificateChainFile;
  cfg.private_key_file          = ssl.privateKeyFile;
  cfg.private_key_password      = ssl.privateKeyPassword;
  cfg.dh_params_file            = ssl.dhParamsFile;
  cfg.client_ca_file            = ssl.clientCaFile;
  cfg.cipher_list               = ssl.cipherList;
  cfg.require_client_cert       = ssl.requireClientCert;
  cfg.handshake_timeout_seconds = ssl.handshakeTimeoutSeconds;
  return cfg;
}

/// Build the outbound-connection TLS configuration from the [ssl] config
/// section. \a _sni_hostname is the name the certificate is expected to carry -
/// normally the host being dialled - and is overridden by an explicit
/// ssl.sniHostname when one is set.
inline Core::SslClientConfig MakeSslClientConfig(Core::Config& _config,
                                                 const std::string& _sni_hostname = "") {
  const auto& ssl = _config.ssl();
  Core::SslClientConfig cfg;
  cfg.verify_peer  = ssl.verifyPeer;
  cfg.ca_file      = ssl.caFile;
  cfg.ca_path      = ssl.caPath;
  cfg.sni_hostname = ssl.sniHostname.empty() ? _sni_hostname : ssl.sniHostname;
  cfg.cipher_list  = ssl.cipherList;
  // The server's own certificate doubles as a client certificate when the peer
  // asks for one (mTLS between servers).
  if (ssl.requireClientCert) {
    cfg.certificate_chain_file = ssl.certificateChainFile;
    cfg.private_key_file       = ssl.privateKeyFile;
    cfg.private_key_password   = ssl.privateKeyPassword;
  }
  return cfg;
}

/*!
 * \brief Apply the server TLS configuration to \a _socket, logging on failure.
 * \param _what human-readable name of the listener, used in the error message.
 * \return false if the certificate or key could not be loaded, in which case
 *         the caller must not proceed to listen().
 */
template <typename SocketT>
bool ApplySslServerConfig(SocketT& _socket, Core::Config& _config, const char* _what) {
  if (!_socket.enable_ssl_server(MakeSslServerConfig(_config))) {
    if (auto log = Core::CLog::GetLogger(Core::log_type::GENERAL).lock()) {
      log->critical("Could not configure TLS for the {} listener. Check ssl.certificateChainFile "
                    "and ssl.privateKeyFile in the server configuration.", _what);
    }
    return false;
  }
  return true;
}

/*!
 * \brief Apply the client TLS configuration to \a _socket, logging on failure.
 * \param _sni_hostname the host being dialled, matched against the peer's
 *        certificate unless ssl.sniHostname overrides it.
 */
template <typename SocketT>
bool ApplySslClientConfig(SocketT& _socket, Core::Config& _config,
                          const std::string& _sni_hostname, const char* _what) {
  if (!_socket.enable_ssl_client(MakeSslClientConfig(_config, _sni_hostname))) {
    if (auto log = Core::CLog::GetLogger(Core::log_type::GENERAL).lock()) {
      log->critical("Could not configure TLS for the {} connection.", _what);
    }
    return false;
  }
  return true;
}

#else  // !USE_SSL

// Plaintext build: nothing to configure, so these succeed unconditionally and
// the call sites stay free of #ifdefs.
template <typename SocketT>
bool ApplySslServerConfig(SocketT&, Core::Config&, const char*) { return true; }

template <typename SocketT>
bool ApplySslClientConfig(SocketT&, Core::Config&, const std::string&, const char*) { return true; }

#endif  // USE_SSL

}  // namespace RoseCommon

#endif /* !_ROSE_SSL_CONFIG_H_ */
