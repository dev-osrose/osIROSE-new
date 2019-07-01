registerNpc(341, {
  walk_speed        = 220,
  run_speed         = 650,
  scale             = 150,
  r_weapon          = 1008,
  l_weapon          = 0,
  level             = 92,
  hp                = 33,
  attack            = 424,
  hit               = 207,
  def               = 385,
  res               = 202,
  avoid             = 138,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 124,
  give_exp          = 90,
  drop_type         = 372,
  drop_money        = 0,
  drop_item         = 57,
  union_number      = 57,
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

registerNpc(933, {
  walk_speed        = 220,
  run_speed         = 550,
  scale             = 180,
  r_weapon          = 1008,
  l_weapon          = 0,
  level             = 91,
  hp                = 3932,
  attack            = 361,
  hit               = 238,
  def               = 328,
  res               = 153,
  avoid             = 122,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 168,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 35,
  sell_tab0         = 35,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
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