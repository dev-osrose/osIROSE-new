#pragma once

#include <memory>

class CMapClient;

struct SocketConnector {
SocketConnector(std::shared_ptr<CMapClient> client) : client_(client) {}

  std::weak_ptr<CMapClient> client_;
};
