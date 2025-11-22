registerNpc(31, {
  walk_speed        = 200,
  run_speed         = 290,
  scale             = 90,
  r_weapon          = 1071,
  l_weapon          = 0,
  level             = 21,
  hp                = 25,
  attack            = 97,
  hit               = 83,
  def               = 65,
  res               = 39,
  avoid             = 39,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 26,
  give_exp          = 32,
  drop_type         = 112,
  drop_money        = 7,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1600,
  npc_type          = 1,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(905, {
  walk_speed        = 150,
  run_speed         = 200,
  scale             = 85,
  r_weapon          = 1071,
  l_weapon          = 0,
  level             = 42,
  hp                = 1343,
  attack            = 131,
  hit               = 122,
  def               = 133,
  res               = 52,
  avoid             = 66,
  attack_spd        = 80,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 30,
  sell_tab0         = 30,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1200,
  npc_type          = 0,
  hit_material_type = 2,
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