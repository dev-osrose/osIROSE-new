#pragma once

#include <sol.hpp>
#include <string>
#include <optional>
#include <unordered_map>

#include "lua_api.h"

// stores npc related lua that can be instanciated at any time
class LuaDb {
    public:
        void store_lua(const std::string& filename, sol::environment env, int npc_id, sol::table npc_data);
        
        struct LuaData {
            LuaData(const sol::table& table) : data(table) {}
            
            int get_walk_speed() { return get_data<int>("walk_speed"); }
            int get_run_speed() { return get_data<int>("run_speed"); }
            int get_scale() { return get_data<int>("scale"); }
            int get_r_weapon() { return get_data<int>("r_weapon"); }
            int get_l_weapon() { return get_data<int>("l_weapon"); }
            int get_level() { return get_data<int>("level"); }
            int get_hp() { return get_data<int>("hp"); }
            int get_attack() { return get_data<int>("attack"); }
            int get_hit() { return get_data<int>("hit"); }
            int get_def() { return get_data<int>("def"); }
            int get_res() { return get_data<int>("res"); }
            int get_avoid() { return get_data<int>("avoid"); }
            int get_attack_spd() { return get_data<int>("attack_spd"); }
            int get_is_magic_damage() { return get_data<int>("is_magic_damage"); }
            int get_ai_type() { return get_data<int>("ai_type"); }
            int get_give_exp() { return get_data<int>("give_exp"); }
            int get_drop_type() { return get_data<int>("drop_type"); }
            int get_drop_money() { return get_data<int>("drop_money"); }
            int get_drop_item() { return get_data<int>("drop_item"); }
            int get_union_number() { return get_data<int>("union_number"); }
            int get_need_summon_count() { return get_data<int>("need_summon_count"); }
            int get_sell_tab0() { return get_data<int>("sell_tab0"); }
            int get_sell_tab1() { return get_data<int>("sell_tab1"); }
            int get_sell_tab2() { return get_data<int>("sell_tab2"); }
            int get_sell_tab3() { return get_data<int>("sell_tab3"); }
            int get_can_target() { return get_data<int>("can_target"); }
            int get_attack_range() { return get_data<int>("attack_range"); }
            int get_npc_type() { return get_data<int>("npc_type"); }
            int get_hit_material_type() { return get_data<int>("hit_material_type"); }
            int get_face_icon() { return get_data<int>("face_icon"); }
            int get_summon_mob_type() { return get_data<int>("summon_mob_type"); }
            int get_quest_type() { return get_data<int>("quest_type"); }
            int get_height() { return get_data<int>("height"); }

            private:
                template <typename T>
                T get_data(const char* name) const {
                    return {static_cast<T>(data[name])};
                }

                const sol::table& data;
        };

        std::weak_ptr<NpcLuaApi> get_lua_api(int npc_id);
        std::optional<LuaData> get_data(int npc_id) const;

        void delete_npc_data(int npc_id);
        void delete_filename(const std::string& filename);

    private:
        

        struct Data {
            std::string filename;
            sol::environment env;
            std::shared_ptr<NpcLuaApi> api;
            sol::table data;
        };

        std::unordered_map<int, Data> data;
        std::unordered_multimap<std::string, int> filename_to_id;
};
