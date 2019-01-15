registerNpc(101, {
  walk_speed        = 0,
  run_speed         = 150,
  scale             = 350,
  r_weapon          = 90,
  l_weapon          = 1003,
  level             = 0,
  hp                = 16,
  attack            = 25,
  hit               = 76,
  def               = 75,
  res               = 54,
  avoid             = 31,
  attack_spd        = 33,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 21,
  drop_type         = 27,
  drop_money        = 147,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
  height            = 0
});
registerNpc(914, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 370,
  r_weapon          = 100,
  l_weapon          = 1003,
  level             = 0,
  hp                = 38,
  attack            = 1431,
  hit               = 120,
  def               = 103,
  res               = 121,
  avoid             = 49,
  attack_spd        = 48,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 29,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
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