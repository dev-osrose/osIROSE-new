#pragma once

#include "system.h"

namespace Systems {

class CallbackSystem : public System {
 public:
  CallbackSystem(SystemManager &manager);
  virtual ~CallbackSystem() = default;

  virtual void update(EntityManager &es, std::chrono::milliseconds dt) override;

  template <typename Func>
  static void add_callback(Entity e, std::chrono::milliseconds time_to_update, Func&& callback) {
      if (!e) return;
      if (!e.component<Callback>()) e.assign<Callback>();
      e.component<Callback>()->callbacks_.emplace_back(std::forward<Func>(callback), time_to_update);
  }
};

}  // namespace Systems
