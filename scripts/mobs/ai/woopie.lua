registerNpc(101, {
  walk_speed        = 150,
  run_speed         = 350,
  scale             = 90,
  r_weapon          = 1003,
  l_weapon          = 0,
  level             = 16,
  hp                = 25,
  attack            = 76,
  hit               = 75,
  def               = 54,
  res               = 31,
  avoid             = 33,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 21,
  give_exp          = 27,
  drop_type         = 147,
  drop_money        = 20,
  drop_item         = 60,
  union_number      = 60,
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

registerNpc(914, {
  walk_speed        = 160,
  run_speed         = 370,
  scale             = 100,
  r_weapon          = 1003,
  l_weapon          = 0,
  level             = 38,
  hp                = 1431,
  attack            = 120,
  hit               = 103,
  def               = 121,
  res               = 49,
  avoid             = 48,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 29,
  sell_tab0         = 29,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
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