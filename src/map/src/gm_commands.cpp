#include <sstream>
#include <string>
#include <tuple>
#include <optional>
#include <functional>

template <std::size_t... Idx>
auto make_index_dispatcher(std::index_sequence<Idx...>) {
    return [] (auto&& f) { (f(std::integral_constant<std::size_t,Idx>{}), ...); };
}

template <std::size_t N>
auto make_index_dispatcher() {
    return make_index_dispatcher(std::make_index_sequence<N>{}); 
}

template <typename Tuple, typename Func>
void for_each(Tuple&& t, Func&& f) {
    constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
    auto dispatcher = make_index_dispatcher<n>();
    dispatcher([&f,&t](auto idx) { f(std::get<idx>(std::forward<Tuple>(t))); });
}

template <typename... Args>
class Parser {
    public:
        Parser(std::stringstream&& ss) {
            size_t index = 0;
            for_each(args, [&ss, &index, this](auto& data) {
                is_arg_ok[index++] = parse(ss, data);
            });
        }

        bool is_good() const {
            return std::all_of(is_arg_ok.cbegin(), is_arg_ok.cend(), [](bool i) {
                return i;
            });
        }

        std::array<bool, sizeof...(Args)> is_arg_ok;
        std::tuple<Args...> args;

    private:
        template <typename T>
        static bool parse(std::stringstream& ss, T& h) {
            return static_cast<bool>(ss >> h);
        }

        template <typename T>
        static bool parse(std::stringstream& ss, std::optional<T>& h) {
            T tmp;
            const bool has_data = ss >> tmp;
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

#define REGISTER_FUNCTION(f) [](EntitySystem& en, Entity e, std::stringstream&& ss) { f(en, e, {std::move(ss)}); }

void item(EntitySystem&, Entity, Parser<int, int>) {
}

void help(EntitySystem& entitySystem, Entity entity, Parser<>) {
    Chat::send_whisper(entitySystem, entity, "help (<required args> [optional args]):");
}

void teleport(EntitySystem&, Entity, Parser<int, int, int, std::optional<uint16_t>>) {
}

void zuly(EntitySystem&, Entity, Parser<int>) {
}

static std::unordered_map<std::string, std::tuple<uint16_t, void(*)(EntitySystem&, Entity, std::stringstream&&), std::string>> commands = {
    {"/item", {100, REGISTER_FUNCTION(item), "Creates an item. Usage: /item <type> <id>"}},
    {"/help", {100, REGISTER_FUNCTION(help), "Prints this help. Usage: /help [command]"}},
    {"/zuly", {100, REGISTER_FUNCTION(zuly), "Adds zulies to your inventory (you can add a negative amount). Usage: /zuly <amount>"}},
    {"/tp", {200, REGISTER_FUNCTION(teleport), "Teleports a player or self. usage: /tp <map_id> <x> <y> [client_id]"}}
};

void execute_gm(EntitySystem& entitySystem, Entity entity, const std::string& message) {
    if (message[0] != '/') {
        return;
    }
    const uint16_t access_level = entitySystem.component<SocketConnector>(entity).access_level;
    std::stringstream ss(message);
    std::string command;
    ss >> command;
    if (const auto it = commands.find(command); it != commands.end() && access_level >= std::get<0>(it->second)) {
        std::get<1>(it->second)(entitySystem, entity, std::move(ss));
    }
}
