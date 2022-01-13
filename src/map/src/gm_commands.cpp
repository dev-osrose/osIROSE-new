#include <sstream>
#include <string>
#include <tuple>
#include <optional>
#include <functional>
#include <unordered_map>

#include "tuple_iterate.h"
#include "entity_system.h"
#include "chat/whisper_chat.h"
#include "components/basic_info.h"
#include "components/client.h"
#include "components/item.h"
#include "components/position.h"
#include "items/inventory.h"
#include "utils/name_to_session.h"

#include "srv_announce_chat.h"
#include "srv_whisper_chat.h"

namespace {
struct all_of {
    std::string data;
    std::string operator=(const std::string& p) {
        data = p;
        return data;
    }

    operator std::string() const {
        return data;
    }
};

template <typename... Args>
class Parser {
    public:
        Parser(std::stringstream&& ss) {
            size_t index = 0;
            Core::for_each(args, [&ss, &index, this](auto& data) {
                is_arg_ok[index++] = parse(ss, data);
            });
        }

        bool is_good() const {
            return std::all_of(is_arg_ok.cbegin(), is_arg_ok.cend(), [](bool i) {
                return i;
            });
        }
        
        bool is_arg_good(size_t index) const {
            return is_arg_ok[index];
        }
        
        template <size_t N>
        decltype(auto) get_arg() {
            return std::get<N>(args);
        }

    private:
        std::array<bool, sizeof...(Args)> is_arg_ok;
        std::tuple<Args...> args;

        template <typename T>
        static bool parse(std::stringstream& ss, T& h) {
            return static_cast<bool>(ss >> h);
        }

        static bool parse(std::stringstream& ss, all_of& h) {
            std::getline(ss, h.data);
            return true;
        }

        template <typename T>
        static bool parse(std::stringstream& ss, std::optional<T>& h) {
            T tmp;
            const bool has_data = static_cast<bool>(ss >> tmp);
            if (has_data) {
                h = tmp;
            }
            return true;
        }
};

template <>
class Parser<> {
    public:
        Parser(std::stringstream&&) {}

        bool is_good() const {
            return true;
        }
};


void item(EntitySystem& entitySystem, Entity entity, Parser<int, int, int, int, int, int> parser) {
    if (!parser.is_good()) {
        Chat::send_whisper(entitySystem, entity, "Error while parsing the command. Usage /item <id> <type> <amount> <refine> <durability> <socket>");
        return;
    }
    const auto item = entitySystem.create_item(parser.get_arg<1>(), parser.get_arg<0>(), parser.get_arg<2>(), parser.get_arg<3>(), parser.get_arg<4>(), parser.get_arg<5>());
    if (item == entt::null) {
        Chat::send_whisper(entitySystem, entity, fmt::format("No existing item ({}, {})", parser.get_arg<0>(), parser.get_arg<1>()));
        return;
    }
    auto& i = entitySystem.get_component<Component::Item>(item);
    i.isAppraised = true;
    switch (Items::add_item(entitySystem, entity, item)) {
        case Items::ReturnValue::NO_SPACE:
            Chat::send_whisper(entitySystem, entity, "Not enough space left in your inventory.");
            break;
        default:
            Chat::send_whisper(entitySystem, entity, fmt::format("Item ({}, {}) added.", parser.get_arg<0>(), parser.get_arg<1>()));
            break;
    }
}

void teleport(EntitySystem& entitySystem, Entity entity, Parser<int, int, int, std::optional<uint16_t>> parser) {
    if (!parser.is_good()) {
        Chat::send_whisper(entitySystem, entity, "Error while parsing the command. Usage /tp <map_id> <x> <y> [client_id]");
        return;
    }
    entitySystem.teleport_entity(entity, parser.get_arg<1>() * 100, parser.get_arg<2>() * 100, parser.get_arg<0>());
}

void zuly(EntitySystem& entitySystem, Entity entity, Parser<int64_t> parser) {
    if (!parser.is_good()) {
        Chat::send_whisper(entitySystem, entity, "Error while parsing the command. Usage /zuly <amount>");
        return;
    }
    if (!Items::add_zuly(entitySystem, entity, parser.get_arg<0>())) {
        Chat::send_whisper(entitySystem, entity, "Error, not enough zulies to remove!");
    }
}

void position(EntitySystem& entitySystem, Entity entity, Parser<>) {
    const auto& pos = entitySystem.get_component<Component::Position>(entity);
    Chat::send_whisper(entitySystem, entity, fmt::format("Position: ({},{})", pos.x, pos.y));
}

void broadcast(EntitySystem& entitySystem, Entity entity, Parser<all_of> parser) {
    if (!parser.is_good()) {
        Chat::send_whisper(entitySystem, entity, "Error while parsing the command. Usage /broadcast <message>");
        return;
    }
    const auto& basic = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto packet = RoseCommon::Packet::SrvAnnounceChat::create(parser.get_arg<0>(), basic.name);
    entitySystem.send_to_maps(packet, {});
}

void whisper(EntitySystem& entitySystem, Entity entity, Parser<std::string, all_of> parser) {
    if (!parser.is_good()) {
        Chat::send_whisper(entitySystem, entity, "Error while parsing the command. Usage /whisper <to> <message>");
        return;
    }
    if (!Utils::name_to_session(parser.get_arg<0>())) {
        Chat::send_whisper(entitySystem, entity, "Error, this character isn't online or doesn't exist");
        return;
    }
    const auto& basic = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto packet = RoseCommon::Packet::SrvWhisperChat::create(basic.name, parser.get_arg<1>());
    entitySystem.send_to_chars(packet, {parser.get_arg<0>()});
}

}

#define REGISTER_FUNCTION(f) [](EntitySystem& en, Entity e, std::stringstream&& ss) { f(en, e, {std::move(ss)}); }

void help(EntitySystem&, Entity, Parser<std::optional<std::string>>);

static const std::unordered_map<std::string, std::tuple<uint16_t, std::function<void(EntitySystem&, Entity, std::stringstream&&)>, std::string>> commands = {
    {"/help", {1, REGISTER_FUNCTION(help), "Prints this help. Usage: /help [command]"}},
    {"/item", {100, REGISTER_FUNCTION(item), "Creates an item. Usage: /item <id> <type> <amount> <refine> <durability> <socket>"}},
    {"/zuly", {100, REGISTER_FUNCTION(zuly), "Adds zulies to your inventory (you can add a negative amount). Usage: /zuly <amount>"}},
    {"/tp", {200, REGISTER_FUNCTION(teleport), "Teleports a player or self. usage: /tp <map_id> <x> <y> [client_id]"}},
    {"/pos", {100, REGISTER_FUNCTION(position), "Returns current position"}},
    {"/broadcast", {100, REGISTER_FUNCTION(broadcast), "Broadcast a message to all maps. Usage /broadcast <message>"}},
    {"/whisper", {100, REGISTER_FUNCTION(whisper), "Whisper a message to any char currently logged in. Usage /whisper <to> <message>"}},
};

static const std::unordered_map<std::string, std::string> aliases = {
    {"/halp", "/help"},
    {"/teleport", "/tp"}
};

void help(EntitySystem& entitySystem, Entity entity, Parser<std::optional<std::string>> parser) {
    if (!parser.is_good()) {
        Chat::send_whisper(entitySystem, entity, "Error while parsing the command. Usage: /help [command]");
        return;
    }
    Chat::send_whisper(entitySystem, entity, "help (<required args> [optional args]):");
    if (!parser.get_arg<0>()) {
        for (const auto& command : commands) {
            Chat::send_whisper(entitySystem, entity, std::get<2>(command.second));
        }
    } else {
        if (const auto it = commands.find(parser.get_arg<0>().value()); it != commands.end()) {
            Chat::send_whisper(entitySystem, entity, std::get<2>(it->second));
        } else {
            Chat::send_whisper(entitySystem, entity, fmt::format("Error, no command {} found.", parser.get_arg<0>().value()));
        }
    }
}

void execute_gm(EntitySystem& entitySystem, Entity entity, const std::string& message) {
    const uint16_t access_level = entitySystem.get_component<Component::Client>(entity).access_level;
    std::stringstream ss(message);
    std::string command;
    ss >> command;
    if (const auto it = aliases.find(command); it != aliases.cend()) {
        command = it->second;
    }
    if (const auto it = commands.find(command); it != commands.end() && access_level >= std::get<0>(it->second)) {
        std::get<1>(it->second)(entitySystem, entity, std::move(ss));
    } else {
        Chat::send_whisper(entitySystem, entity, "Error, no known command/not enough permissions");
    }
}
