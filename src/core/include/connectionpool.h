#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <utility>
#include <unordered_map>

#include "singleton.h"
#include "throwassert.h"

namespace Core {

template <typename T>
class ConnectionPool : public Singleton<ConnectionPool<T>> {
    public:
        class Wrapper {
            public:
                Wrapper(T& conn, const std::string &name,
                        ConnectionPool<T> *callback)
                    : _conn(conn), _name(name), _callback(callback) {}
                ~Wrapper() {
                    _callback->unMark(_name, &_conn);
                }

                template <typename U>
                auto operator()(const U& u) {
                    return _conn(u);
                }

                T* operator->() {
                    return &_conn;
                }

                T& get() {
                    return _conn;
                }

            private:
                T &_conn;
                const std::string &_name;
                ConnectionPool<T> *_callback;
        };

        template <typename U>
        void addConnector(const std::string &name, U factory) { 
            const std::function<std::unique_ptr<T>()> tmp = static_cast<decltype(tmp)>(factory);
            _factories[name] = tmp;
        }

        Wrapper getConnection(const std::string &name) {
            throw_assert(_factories.count(name), name << " cannot be constructed, no factory available");
            for (auto &conn : _connections[name])
                if (!conn.first) {
                    conn.first = true;
                    return Wrapper(*conn.second.get(), name, this);
                }
            _connections[name].emplace_back(std::make_pair(true, _factories[name]()));
            return Wrapper(*_connections[name].back().second.get(), name, this);
        }

    protected:
        friend Wrapper;
        void unMark(const std::string &name, T* value) {
            for (auto &conn : _connections[name])
                if (conn.first == true && conn.second.get() == value) {
                    conn.first = false;
                    break;
                }
        }

    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<T>()>> _factories;
        // false -> available
        std::unordered_map<std::string, std::vector<std::pair<bool, std::unique_ptr<T>>>> _connections;
};
}
