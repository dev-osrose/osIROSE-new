registerNpc(211, {
  walk_speed        = 450,
  run_speed         = 750,
  scale             = 360,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 70,
  hp                = 182,
  attack            = 387,
  hit               = 214,
  def               = 266,
  res               = 191,
  avoid             = 86,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 60,
  give_exp          = 724,
  drop_type         = 206,
  drop_money        = 1,
  drop_item         = 80,
  union_number      = 80,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 10,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(406, {
  walk_speed        = 260,
  run_speed         = 700,
  scale             = 380,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 76,
  hp                = 219,
  attack            = 400,
  hit               = 248,
  def               = 276,
  res               = 164,
  avoid             = 117,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 75,
  give_exp          = 655,
  drop_type         = 306,
  drop_money        = 1,
  drop_item         = 90,
  union_number      = 90,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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