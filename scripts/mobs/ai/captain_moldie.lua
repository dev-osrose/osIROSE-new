registerNpc(114, {
  walk_speed        = 200,
  run_speed         = 500,
  scale             = 116,
  r_weapon          = 1016,
  l_weapon          = 0,
  level             = 42,
  hp                = 44,
  attack            = 215,
  hit               = 133,
  def               = 135,
  res               = 86,
  avoid             = 51,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 24,
  give_exp          = 80,
  drop_type         = 153,
  drop_money        = 30,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
  npc_type          = 9,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(180, {
  walk_speed        = 200,
  run_speed         = 700,
  scale             = 140,
  r_weapon          = 1016,
  l_weapon          = 0,
  level             = 45,
  hp                = 66,
  attack            = 231,
  hit               = 152,
  def               = 144,
  res               = 92,
  avoid             = 55,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 262,
  give_exp          = 90,
  drop_type         = 153,
  drop_money        = 30,
  drop_item         = 39,
  union_number      = 39,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
  npc_type          = 9,
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