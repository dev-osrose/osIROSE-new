registerNpc(271, {
  walk_speed        = 0,
  run_speed         = 210,
  scale             = 850,
  r_weapon          = 110,
  l_weapon          = 1015,
  level             = 0,
  hp                = 51,
  attack            = 28,
  hit               = 228,
  def               = 131,
  res               = 146,
  avoid             = 98,
  attack_spd        = 78,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 328,
  drop_type         = 40,
  drop_money        = 226,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 58,
  sell_tab0         = 58,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 14,
  height            = 0
});
registerNpc(928, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 410,
  r_weapon          = 110,
  l_weapon          = 1015,
  level             = 0,
  hp                = 62,
  attack            = 2437,
  hit               = 206,
  def               = 149,
  res               = 199,
  avoid             = 79,
  attack_spd        = 75,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 33,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 0,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 0,
  height            = 0
});
function OnInit(entity)
  return true
end

function OnCreate(entity)
  return true
end

function OnDelete(entity)
  return true
end

function OnDead(entity)
end

function OnDamaged(entity)
end