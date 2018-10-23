#pragma once

#include <memory>

class CMapClient;

struct SocketConnector {
SocketConnector(std::shared_ptr<CMapClient> client) : client_(client) {}

  std::weak_ptr<CMapClient> client_;
  uint16_t access_level_ = 100;
};
