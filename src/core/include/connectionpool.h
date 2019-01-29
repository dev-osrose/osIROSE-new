#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <vector>
#include <utility>

#include "singleton.h"
#include "tuple_index.h"

namespace Core {

template <typename... Connectors>
class ConnectionPool : public Singleton<ConnectionPool<Connectors...>> {
    public:
        template <typename C>
        class Wrapper {
            public:
                Wrapper(typename C::type& conn, ConnectionPool<Connectors...>* callback)
                : conn(conn), callback(callback) {}

                ~Wrapper() {
                    callback->unMark<C>(&conn);
                }

                template <typename U>
                auto operator()(const U& u) {
                    return conn(u);
                }

                typename C::type* operator->() {
                    return &conn;
                }

                typename C::type& get() {
                    return conn;
                }

            private:
                typename C::type& conn;
                ConnectionPool<Connectors...>* callback;
        };

        template <typename C, typename T>
        void addConnector(const T& factory) {
            constexpr size_t index = Core::Index<C, std::tuple<Connectors...>>::value;
            std::get<index>(connectors) = factory;
        }

        template <typename C>
        Wrapper<C> getConnection() {
            constexpr size_t index = Core::Index<C, std::tuple<Connectors...>>::value;
            auto& connection = std::get<index>(connections);
            for (auto& conn : connection) {
                if (!conn.first) {
                    conn.first = true;
                    return Wrapper<C>{*conn.second.get(), this};
                }
            }
            auto& factory = std::get<index>(connectors);
            connection.emplace_back(std::make_pair(true, factory()));
            return Wrapper<C>{*connection.back().second.get(), this};
        }

    protected:
        template <typename T>
        friend class Wrapper;

        template <typename C>
        void unMark(typename C::type* value) {
            constexpr size_t index = Index<C, std::tuple<Connectors...>>::value;
            auto& connection = std::get<index>(connections);
            for (auto &conn : connection) {
                if (conn.first == true && conn.second.get() == value) {
                    conn.first = false;
                    break;
                }
            }
        }

    private:

        template <typename T>
        using function_type = std::function<std::unique_ptr<typename T::type>()>;

        template <typename T>
        using connection_type = std::pair<bool, std::unique_ptr<typename T::type>>;

        std::tuple<function_type<Connectors>...> connectors;
        std::tuple<std::vector<connection_type<Connectors>>...> connections;
};
}
