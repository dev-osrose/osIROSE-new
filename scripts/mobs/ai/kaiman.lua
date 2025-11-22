registerNpc(141, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 140,
  r_weapon          = 103,
  l_weapon          = 0,
  level             = 59,
  hp                = 29,
  attack            = 265,
  hit               = 145,
  def               = 171,
  res               = 116,
  avoid             = 89,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 114,
  give_exp          = 52,
  drop_type         = 165,
  drop_money        = 16,
  drop_item         = 48,
  union_number      = 48,
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

registerNpc(920, {
  walk_speed        = 200,
  run_speed         = 450,
  scale             = 170,
  r_weapon          = 104,
  l_weapon          = 0,
  level             = 70,
  hp                = 2572,
  attack            = 249,
  hit               = 184,
  def               = 215,
  res               = 77,
  avoid             = 79,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 167,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 34,
  sell_tab0         = 34,
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