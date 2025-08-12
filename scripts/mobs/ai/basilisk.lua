registerNpc(370, {
  walk_speed        = 190,
  run_speed         = 700,
  scale             = 130,
  r_weapon          = 1084,
  l_weapon          = 0,
  level             = 87,
  hp                = 31,
  attack            = 372,
  hit               = 192,
  def               = 266,
  res               = 212,
  avoid             = 161,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 128,
  give_exp          = 67,
  drop_type         = 368,
  drop_money        = 0,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 800,
  npc_type          = 5,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(372, {
  walk_speed        = 0,
  run_speed         = 0,
  scale             = 0,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 0,
  hp                = 0,
  attack            = 0,
  hit               = 0,
  def               = 0,
  res               = 0,
  avoid             = 0,
  attack_spd        = 0,
  is_magic_damage   = 0,
  ai_type           = 0,
  give_exp          = 4,
  drop_type         = 388,
  drop_money        = 0,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1300,
  npc_type          = 9,
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