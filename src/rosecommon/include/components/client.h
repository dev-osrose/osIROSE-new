#pragma once

#include <memory>

class CMapClient;

namespace Component {
struct Client {
  std::weak_ptr<CMapClient> client;  
};
}
