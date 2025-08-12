registerNpc(81, {
  walk_speed        = 150,
  run_speed         = 420,
  scale             = 70,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 20,
  hp                = 25,
  attack            = 93,
  hit               = 81,
  def               = 63,
  res               = 37,
  avoid             = 38,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 3,
  give_exp          = 30,
  drop_type         = 136,
  drop_money        = 15,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 180,
  npc_type          = 1,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(910, {
  walk_speed        = 190,
  run_speed         = 380,
  scale             = 80,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 41,
  hp                = 1603,
  attack            = 127,
  hit               = 105,
  def               = 143,
  res               = 63,
  avoid             = 48,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 165,
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
  attack_range      = 200,
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