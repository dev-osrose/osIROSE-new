registerNpc(31, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 290,
  r_weapon          = 90,
  l_weapon          = 1071,
  level             = 0,
  hp                = 21,
  attack            = 25,
  hit               = 97,
  def               = 83,
  res               = 65,
  avoid             = 39,
  attack_spd        = 39,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 26,
  drop_type         = 32,
  drop_money        = 112,
  drop_item         = 7,
  union_number      = 7,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1600,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 11,
  height            = 0
});
registerNpc(905, {
  walk_speed        = 0,
  run_speed         = 150,
  scale             = 200,
  r_weapon          = 85,
  l_weapon          = 1071,
  level             = 0,
  hp                = 42,
  attack            = 1343,
  hit               = 131,
  def               = 122,
  res               = 133,
  avoid             = 52,
  attack_spd        = 66,
  is_magic_damage   = 80,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 30,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1200,
  hit_material_type = 0,
  face_icon         = 2,
  summon_mob_type   = 2,
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