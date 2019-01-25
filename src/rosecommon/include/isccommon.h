#pragma once

#include <cstdint>
#include <string>

namespace RoseCommon {
  namespace Isc {
    enum ServerType : uint8_t {
      LOGIN = 0,
        CHAR = 1,
        NODE = 2,
        MAP_MASTER = 3,
        MAP_WORKER = 4
        };

    std::string serverTypeName(ServerType type);

    bool operator==(const ServerType &type, const uint32_t &rhs);
    bool operator==(const uint32_t &lhs, const ServerType &type);
  }
}
