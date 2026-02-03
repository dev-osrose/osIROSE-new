#pragma once

#include <string>
#include <optional>
#include <cstdint>

namespace Utils {

std::optional<uint32_t> name_to_session(const std::string& name);

}
