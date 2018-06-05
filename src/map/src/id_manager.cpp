#include "id_manager.h"

uint16_t IdManager::get_free_id() {
  if (_free_ids.size()) {
    return _free_ids.extract(_free_ids.begin()).value();
  }
  return _max_id++;
}

void IdManager::release_id(uint16_t id) {
  _free_ids.emplace(id);
}
