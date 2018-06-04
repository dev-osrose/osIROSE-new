#pragma once

class IdManager {
    public:
        IdManager();
        IdManager(const IdManager&) = delete;
        IdManager(const IdManager&&) = default;
        ~IdManager() = default;
        
        uint16_t get_free_id();
        void release_id(uint16_t id);
};
