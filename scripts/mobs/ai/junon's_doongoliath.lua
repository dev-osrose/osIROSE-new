registerNpc(211, {
  walk_speed        = 0,
  run_speed         = 450,
  scale             = 750,
  r_weapon          = 360,
  l_weapon          = 0,
  level             = 0,
  hp                = 70,
  attack            = 182,
  hit               = 387,
  def               = 214,
  res               = 266,
  avoid             = 191,
  attack_spd        = 86,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 60,
  drop_type         = 724,
  drop_money        = 206,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 80,
  sell_tab0         = 80,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 15,
  height            = 0
});
registerNpc(406, {
  walk_speed        = 0,
  run_speed         = 260,
  scale             = 700,
  r_weapon          = 380,
  l_weapon          = 0,
  level             = 0,
  hp                = 76,
  attack            = 219,
  hit               = 400,
  def               = 248,
  res               = 276,
  avoid             = 164,
  attack_spd        = 117,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 75,
  drop_type         = 655,
  drop_money        = 306,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 90,
  sell_tab0         = 90,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 19,
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