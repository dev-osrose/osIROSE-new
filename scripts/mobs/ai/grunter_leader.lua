registerNpc(138, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 140,
  r_weapon          = 1021,
  l_weapon          = 0,
  level             = 53,
  hp                = 36,
  attack            = 258,
  hit               = 150,
  def               = 162,
  res               = 79,
  avoid             = 86,
  attack_spd        = 115,
  is_magic_damage   = 0,
  ai_type           = 115,
  give_exp          = 70,
  drop_type         = 163,
  drop_money        = 35,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 8,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(919, {
  walk_speed        = 220,
  run_speed         = 490,
  scale             = 185,
  r_weapon          = 1021,
  l_weapon          = 0,
  level             = 65,
  hp                = 2736,
  attack            = 234,
  hit               = 186,
  def               = 213,
  res               = 91,
  avoid             = 83,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 167,
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
  attack_range      = 500,
  npc_type          = 0,
  hit_material_type = 0,
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