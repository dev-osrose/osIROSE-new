registerNpc(271, {
  walk_speed        = 210,
  run_speed         = 850,
  scale             = 110,
  r_weapon          = 1015,
  l_weapon          = 0,
  level             = 51,
  hp                = 28,
  attack            = 228,
  hit               = 131,
  def               = 146,
  res               = 98,
  avoid             = 78,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 328,
  give_exp          = 40,
  drop_type         = 226,
  drop_money        = 10,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(928, {
  walk_speed        = 180,
  run_speed         = 410,
  scale             = 110,
  r_weapon          = 1015,
  l_weapon          = 0,
  level             = 62,
  hp                = 2437,
  attack            = 206,
  hit               = 149,
  def               = 199,
  res               = 79,
  avoid             = 75,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 33,
  sell_tab0         = 33,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
  npc_type          = 0,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
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