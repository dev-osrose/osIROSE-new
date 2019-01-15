registerNpc(196, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 640,
  r_weapon          = 160,
  l_weapon          = 40,
  level             = 3,
  hp                = 93,
  attack            = 59,
  hit               = 390,
  def               = 236,
  res               = 485,
  avoid             = 226,
  attack_spd        = 80,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 262,
  drop_type         = 73,
  drop_money        = 232,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 37,
  sell_tab0         = 37,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});
registerNpc(276, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 850,
  r_weapon          = 140,
  l_weapon          = 38,
  level             = 2,
  hp                = 70,
  attack            = 37,
  hit               = 288,
  def               = 178,
  res               = 262,
  avoid             = 159,
  attack_spd        = 57,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 116,
  drop_type         = 47,
  drop_money        = 231,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 41,
  sell_tab0         = 41,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 14,
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