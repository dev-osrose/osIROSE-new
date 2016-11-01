#pragma once

#include <map>
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
            for (auto it = dispatch_.begin(); it != dispatch_.end(); ++it) {
                if (dynamic_cast<T>(it->second)) {
                    dispatch_.erase(it);
                    return;
                }
            }
        }

        template <typename T>
        T* get() {
            for (auto &it : systems_) {
                T *tmp;
                if ((tmp = dynamic_cast<T*>(it.get())))
                    return tmp;
            }
            return nullptr;
        }

        void update(double dt);

        bool dispatch(Entity entity, const RoseCommon::CRosePacket &packet) {
            auto res = dispatch_.find(packet.type());
            if (res != dispatch_.end()) {
                for (auto it : res->second)
                    it(entity, packet);
                if (res->second.size())
                    return true;
            }
            return false;
        }

        template <class T>
        void registerDispatcher(RoseCommon::ePacketType type, void(T::*method)(Entity, const RoseCommon::CRosePacket&)) {
            auto res = get<T>();
            if (!res)
                return;
            dispatch_[type].emplace_back([object = res, method](Entity entity, const RoseCommon::CRosePacket &packet) {
                    (object ->* method)(entity, packet);
                    });
        }

    private:
        EntityManager &entityManager_;
        std::vector<std::unique_ptr<System>> systems_;
        std::map<RoseCommon::ePacketType, std::vector<std::function<void(Entity, const RoseCommon::CRosePacket&)>>> dispatch_;
};

