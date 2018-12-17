#pragma once

#include <memory>

class CMapClient;

namespace Component {
struct Client {
  std::shared_ptr<CMapClient> client;  
};
}
