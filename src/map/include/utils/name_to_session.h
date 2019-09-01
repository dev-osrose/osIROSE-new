#pragma once

#include <string>
#include <optional>

namespace Utils {

std::optional<uint32_t> name_to_session(const std::string& name);
std::optional<std::string> id_to_name(const uint32_t charId);

}
