#pragma once

#include <memory>

class CMapClient;

namespace Component {
struct Client {
  Client(std::weak_ptr<CMapClient> client, uint16_t access_level) : client(client), access_level(access_level) {}
  Client(const Client&) = default;
  Client(Client&& other) : client(std::move(other.client)), access_level(other.access_level) {}
  ~Client() = default;
  Client& operator=(const Client&) = default;
  Client& operator=(Client&& other) {
      Client tmp(std::move(other));
      std::swap(tmp, *this);
      return *this;
  }

  std::weak_ptr<CMapClient> client;
  uint16_t access_level;
};
}
