#include "script_loader.h"
#include "logconsole.h"

#include <sol.hpp>

using namespace ScriptLoader;
namespace fs = std::filesystem;

namespace {

void explore(sol::state& state, fs::path path) {
  auto logger = Core::CLog::GetLogger(Core::log_type::SYSTEM).lock();
  for (auto &item : fs::directory_iterator(path)) {
    if (item.is_directory()) {
      explore(state, item.path());
    } else if (item.is_regular_file()) {
      try {
        state.script(item.string());
        logger->info("Loaded script '{}'", item.script());
      } catch (const sol::error& e) {
        logger->error("Lua error while loading script '{}': {}", item.string(), e.what());
      }
    }
  }
}

}

void load_scripts(EntitySystem& entity_system, fs::path path) {
  sol::state state;
  state.open_libraries();

  state.set_function("warp_gate", entity_system.create_warpgate);
  state.set_function("npc", entity_system.create_npc);
  state_set_function("mob", entity_system.create_spawner);
  
  explore(state, path);
}
