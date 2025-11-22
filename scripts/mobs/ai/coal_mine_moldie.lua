registerNpc(112, {
  walk_speed        = 200,
  run_speed         = 460,
  scale             = 105,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 38,
  hp                = 32,
  attack            = 154,
  hit               = 119,
  def               = 145,
  res               = 81,
  avoid             = 30,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 107,
  give_exp          = 45,
  drop_type         = 151,
  drop_money        = 25,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 2,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(179, {
  walk_speed        = 200,
  run_speed         = 700,
  scale             = 120,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 42,
  hp                = 49,
  attack            = 170,
  hit               = 133,
  def               = 159,
  res               = 89,
  avoid             = 33,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 267,
  give_exp          = 45,
  drop_type         = 152,
  drop_money        = 25,
  drop_item         = 32,
  union_number      = 32,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 2,
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