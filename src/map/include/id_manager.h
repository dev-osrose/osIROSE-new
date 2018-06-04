#pragma once

#include <unordered_set>

class IdManager {
    public:
        IdManager();
        IdManager(const IdManager&) = delete;
        IdManager(const IdManager&&) = default;
        ~IdManager() = default;
        
        uint16_t get_free_id();
        void release_id(uint16_t id);
    
    private:
        std::unordered_set<uint16_t> _free_ids;
        uint16_t _max_id;
};
