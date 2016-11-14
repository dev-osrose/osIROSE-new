#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "crosepacket.h"
#include "epackettype.h"
#include "entityComponents.h"
#include <typeindex>
#include <algorithm>
#include <type_traits>

using RoseCommon::to_underlying;

namespace Systems {
class System;
}

using Systems::System;

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
            auto res = dispatch_.equal_range(to_underlying(packet.type()));
            if (res.first == dispatch_.end())
                return false;
            for (auto it = res.first; it != res.second;) {
                auto &func = it->second;
                if (!func(entity, packet))
                    dispatch_.erase(it++);
                else
                    ++it;
            }
            if (std::distance(res.first, res.second) > 0)
                return true;
            return false;
        }

        // FIXME : No possibility to unregister as of now
        template <class T, class U>
        void registerDispatcher(RoseCommon::ePacketType type, void(T::*method)(Entity, const U&)) {
            std::shared_ptr<System> res;
            try {
                res = systems_.at(typeid(T));
            } catch (std::out_of_range) {
                return;
            }
            dispatch_.emplace(to_underlying(type), [object = std::weak_ptr<System>(res), method](Entity entity, const RoseCommon::CRosePacket &packet) {
                    if (auto system = dynamic_cast<T*>(object.lock().get())) {
                        (system ->* method)(entity, dynamic_cast<const U&>(packet));
                        return true;
                    } else
                        return false;
                    });
        }

    private:
        EntityManager &entityManager_;
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems_;
        std::unordered_multimap<std::underlying_type_t<RoseCommon::ePacketType>,
		std::function<bool(Entity, const RoseCommon::CRosePacket&)>> dispatch_;
};

