#include "utils/name_to_session.h"
#include "connection.h"

namespace Utils {

std::optional<uint32_t> name_to_session(const std::string& name) {
    Core::SessionTable sessions{};
    Core::CharacterTable characters{};

    auto conn = Core::connectionPool.getConnection<Core::Osirose>();

    const auto res = conn(
            sqlpp::select(sessions.id).from(sessions.join(characters).on(characters.id == sessions.charid))
                .where(characters.name == name)
        );
    if (res.empty()) {
        return {};
    }
    return {static_cast<uint32_t>(res.front().id)};
}

std::optional<std::string> id_to_name(const uint32_t charId) {
    Core::CharaterTable characters{};
    
    auto conn = Core::connectionPool.getConnection<Core::Osirose>();
    
    const auto res = conn(sqlpp::select(characters.name).from(characters).where(characters.id == charId));
    if (res.empty()) {
        return {};
    }
    return {std::string{res.front().name}};
}

}
