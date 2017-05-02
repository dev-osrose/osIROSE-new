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
        void addConnector(const std::string &name, T factory) {
            std::function<decltype(factory())()> tmp = static_cast<std::function<decltype(factory())()>>(factory);
            std::get<std::pair<std::string, decltype(tmp)>>(_factories) = std::make_pair(name, tmp);
        }

        decltype(auto) getConnection(const std::string &name) {
            constexpr std::size_t id = findFactoryId(_factories, name);
            const auto factory = std::get<std::integral_constant<std::size_t, id>{}>(_factories).second;
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
        template <class Tuple>
        static constexpr std::size_t findFactoryId(Tuple &tup, const std::string &name) {
            std::size_t i = 0;
            for_each_in_tuple(tup, [&](const std::size_t is, const auto pair) {
                    i = name == pair.first ? is : i;
                    });
            return i;
        }

        std::tuple<std::pair<std::string, std::function<Connectors()>>...> _factories;
        // true -> available
        std::array<std::vector<std::pair<bool, std::experimental::any>>, sizeof...(Connectors)> _connections;
};
}
