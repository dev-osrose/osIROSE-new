#include "systems/callbacksystem.h"
#include <stack>

using namespace Systems;
using namespace RoseCommon;

CallbackSystem::CallbackSystem(SystemManager &manager) : System(manager) {}

void CallbackSystem::update(EntityManager &es, std::chrono::milliseconds dt) {
    for (Entity e : es.entities_with_components<Callback>()) {
        auto call = e.component<Callback>();
        std::stack<std::vector<CallbackImp>::const_iterator> to_delete;
        for (auto it = call->callbacks_.begin(); it != call->callbacks_.end(); ++it) {
            it->time_elapsed_ += dt;
            if (it->time_elapsed_ >= it->time_to_update_) {
                if (!(it->callback_)(manager_))
                    to_delete.push(it);
                else
                    it->time_elapsed_ = 0ms;
            }
        }
        while (to_delete.size()) {
            call->callbacks_.erase(to_delete.top());
            to_delete.pop();
        }
        if (!call->callbacks_.size()) {
            e.remove<Callback>();
        }
    }
}
