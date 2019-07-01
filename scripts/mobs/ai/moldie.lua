registerNpc(111, {
  walk_speed        = 180,
  run_speed         = 520,
  scale             = 95,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 37,
  hp                = 26,
  attack            = 165,
  hit               = 107,
  def               = 106,
  res               = 68,
  avoid             = 59,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 21,
  give_exp          = 47,
  drop_type         = 150,
  drop_money        = 20,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(915, {
  walk_speed        = 180,
  run_speed         = 410,
  scale             = 120,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 53,
  hp                = 2109,
  attack            = 168,
  hit               = 127,
  def               = 184,
  res               = 80,
  avoid             = 61,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 31,
  sell_tab0         = 31,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
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