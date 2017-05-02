#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <utility>
#include <type_traits>
#include <tuple>
#include <experimental/any>

#include "singleton.h"
#include "iterate.h"

namespace Core {

template <typename... Connectors>
class ConnectionPool : public Singleton<ConnectionPool<Connectors...>> {
    public:
        template <typename T>
        void addConnector(T factory) {
            std::function<decltype(factory())()> tmp = static_cast<std::function<decltype(factory())()>>(factory);
            std::get<decltype(tmp)>(_factories) = tmp;
        }

        template <typename T, typename... Args>
        static constexpr std::size_t getId(T(*)(Args...)) {
            return IndexOfType<std::function<T()>, decltype(_factories)>::value;
        }

        template <std::size_t id>
        auto getConnection() {
            const auto factory = std::get<std::integral_constant<std::size_t, id>{}>(_factories);
            for (auto &connection : _connections[id])
                if (connection.first) {
                    connection.first = false;
                    decltype(factory()) &res = std::experimental::any_cast(connection.second);
                    return res;
                }
            _connections[id].emplace_back(std::make_pair<bool, std::experimental::any>(false, factory()));
            decltype(factory()) &res = std::experimental::any_cast(_connections[id].back().second);
            return res;
        }

    private:
        std::tuple<std::function<Connectors()>...> _factories;
        // true -> available
        std::array<std::vector<std::pair<bool, std::experimental::any>>, sizeof...(Connectors)> _connections;
};
}
