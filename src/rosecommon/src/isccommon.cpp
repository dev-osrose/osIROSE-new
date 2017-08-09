#include "isccommon.h"

namespace RoseCommon {
  namespace Isc {

std::string serverTypeName(ServerType type) {
  switch (type) {
  case ServerType::LOGIN:
    return "LOGIN";
  case ServerType::CHAR:
    return "CHAR";
  case ServerType::NODE:
    return "NODE";
  case ServerType::MAP_MASTER:
    return "MAP_MASTER";
  case ServerType::MAP_WORKER:
    return "MAP_WORKER";
  default:
    return "";
  }
}

bool operator==(const ServerType &type, const uint32_t &rhs) {
  return (static_cast<uint8_t>(type) == rhs);
}

bool operator==(const uint32_t &lhs, const ServerType &type) {
  return (lhs == static_cast<uint8_t>(type));
}

 }
}
