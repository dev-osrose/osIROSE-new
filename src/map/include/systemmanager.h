// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file systemmanager
 * \brief Contains the SystemManager class that manages all systems.
 *
 * \author L3nn0x
 * \date october 2016
 */
#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "crosepacket.h"
#include "epackettype.h"
#include "entitycomponents.h"
#include <typeindex>
#include <algorithm>
#include <type_traits>

using RoseCommon::to_underlying;

namespace Systems {
class System;
}

class CMapClient;
class EntitySystem;

using Systems::System;

/*!
 * \class SystemManager
 * \brief handles all systems that should interact with entities
 *
 * \author L3nn0x
 * \date october 2016
 */
class SystemManager {
    public:
        SystemManager(EntitySystem &es) : entitySystem_(es) {}

        template <typename T, typename... Args>
        T& add(Args&&... args) {
            systems_.emplace(typeid(T), std::make_unique<T>(*this, std::forward<Args>(args)...));
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
            } catch (const std::out_of_range&) {}
            return nullptr;
        }

        void update(double dt);

        bool wouldDispatch(const RoseCommon::CRosePacket &packet) {
            auto res = dispatch_.equal_range(to_underlying(packet.type()));
            if (std::distance(res.first, res.second) > 0)
                return true;
            return false;
        }

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

        // FIXME : No possibility to manually unregister as of now
        template <class T, class U>
        void registerDispatcher(RoseCommon::ePacketType type, void(T::*method)(CMapClient&, Entity, const U&)) {
            dispatch_.emplace(to_underlying(type), [this, method](Entity entity, const RoseCommon::CRosePacket &packet) {
                    if (auto *system = this->get<T>()) {
                        if (!entity)
                            return false;
                        if (auto socket = getClient(entity)) {
                            (system ->* method)(*socket, entity, dynamic_cast<const U&>(packet));
                            return true;
                        }
                    } else
                        return false;
                    return false;
                    });
        }

        Entity getEntity(const std::string &name) const;
        Entity getEntity(uint32_t charId) const;

        Entity buildItem(Entity creator, RoseCommon::Item&&);

        EntityManager &getEntityManager();

    private:
        EntitySystem &entitySystem_;
        std::unordered_map<std::type_index, std::unique_ptr<System>> systems_;
        std::unordered_multimap<std::underlying_type_t<RoseCommon::ePacketType>,
		std::function<bool(Entity, const RoseCommon::CRosePacket&)>> dispatch_;
};
