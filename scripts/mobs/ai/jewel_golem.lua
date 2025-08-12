registerNpc(161, {
  walk_speed        = 250,
  run_speed         = 700,
  scale             = 200,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 47,
  hp                = 74,
  attack            = 253,
  hit               = 164,
  def               = 168,
  res               = 102,
  avoid             = 57,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 9,
  give_exp          = 167,
  drop_type         = 179,
  drop_money        = 1,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
  npc_type          = 10,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(924, {
  walk_speed        = 250,
  run_speed         = 500,
  scale             = 260,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 62,
  hp                = 2593,
  attack            = 222,
  hit               = 179,
  def               = 202,
  res               = 87,
  avoid             = 79,
  attack_spd        = 80,
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
  attack_range      = 250,
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