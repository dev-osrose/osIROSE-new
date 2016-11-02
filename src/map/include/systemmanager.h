#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "crosepacket.h"
#include "epackettype.h"
#include "entityComponents.h"

class System;

class SystemManager {
    public:
        SystemManager(EntityManager &es) : entityManager_(es) {}

        template <typename T, typename... Args>
        T& add(Args&&... args) {
            systems_.emplace_back(new T(*this, std::forward<Args>(args)...));
            return *dynamic_cast<T*>(systems_.back().get());
        }

        template <typename T>
        void remove() {
            for (auto it = systems_.begin(); it != systems_.end(); ++it) {
                if (dynamic_cast<T*>(it->get())) {
                    systems_.erase(it);
                    break;
                }
            }
        }

        template <typename T>
        T* get() {
            for (auto &it : systems_)
                if (T *tmp = dynamic_cast<T*>(it.get()))
                    return tmp;
            return nullptr;
        }

        void update(double dt);

        bool dispatch(Entity entity, const RoseCommon::CRosePacket &packet) {
            auto res = dispatch_.find(packet.type());
            if (res != dispatch_.end()) {
                for (auto it : res->second) {
                    try {
                        it(entity, packet);
                    } catch (std::bad_cast) {}
                }
                if (res->second.size())
                    return true;
            }
            return false;
        }

        template <class T>
        void registerDispatcher(RoseCommon::ePacketType type, void(T::*method)(Entity, const RoseCommon::CRosePacket&)) {
            std::shared_ptr<System> res;
            for (auto &it : systems_)
                if (dynamic_cast<T*>(it.get()))
                    res = it;
            if (!res)
                return;
            dispatch_[type].emplace_back([type, pos = dispatch_[type].size(), this, object = std::weak_ptr<System>(res), method](Entity entity, const RoseCommon::CRosePacket &packet) {
                    if (auto system = dynamic_cast<T*>(object.lock().get()))
                        (system ->* method)(entity, packet);
                    else
                        this->unregisterDispatcher(type, pos);
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
        std::vector<std::shared_ptr<System>> systems_;
        std::unordered_map<RoseCommon::ePacketType, std::vector<std::function<void(Entity, const RoseCommon::CRosePacket&)>>> dispatch_;
};

