#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "crosepacket.h"
#include "epackettype.h"
#include "entityComponents.h"
#include <typeindex>
#include <algorithm>

class System;

class SystemManager {
    public:
        SystemManager(EntityManager &es) : entityManager_(es) {}

        template <typename T, typename... Args>
        T& add(Args&&... args) {
            systems_[typeid(T)] = std::make_shared<T>(*this, std::forward<Args>(args)...);
            return *dynamic_cast<T*>(systems_.at(typeid(T)).get());
        }

        template <typename T>
        void remove() {
            systems_.erase(typeid(T));
        }

        template <typename T>
        T* get() {
            try {
                return dynamic_cast<T*>(systems_.at(typeid(T)).get());
            } catch (std::out_of_range) {}
            return nullptr;
        }

        void update(double dt);

        bool dispatch(Entity entity, const RoseCommon::CRosePacket &packet) {
            try {
                auto res = dispatch_.at(packet.type());
            } catch (std::out_of_range) {
                return false;
            }
            res->second.erase(std::remove_if(res->second.begin(), res->second.end(), [entity, &packet] (auto &func) {
                        try {
                            return !func(entity, packet);
                        } catch (std::bad_cast) {}
                        return true;
                    }), res->second.end());
            if (res->second.size())
                return true;
            return false;
        }

        template <class T, class U>
        void registerDispatcher(RoseCommon::ePacketType type, void(T::*method)(Entity, const U&)) {
            std::shared_ptr<System> res;
            try {
                res = systems_.at(typeid(T));
            } catch (std::out_of_range) {
                return;
            }
            dispatch_[type].emplace_back([object = std::weak_ptr<System>(res), method](Entity entity, const RoseCommon::CRosePacket &packet) {
                    if (auto system = dynamic_cast<T*>(object.lock().get())) {
                        (system ->* method)(entity, dynamic_cast<const U&>(packet));
                        return true;
                    } else
                        return false;
                    });
        }

        void unregisterDispatcher(RoseCommon::ePacketType type) {
            dispatch_.erase(type);
        }

        void unregisterDispatcher(RoseCommon::ePacketType type, size_t pos) {
            dispatch_[type].erase(dispatch_[type].begin() + pos);
        }

    private:
        EntityManager &entityManager_;
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems_;
        // TODO : use a std::unordered_multimap
        std::unordered_map<RoseCommon::ePacketType, std::vector<std::function<bool(Entity, const RoseCommon::CRosePacket&)>>> dispatch_;
};

