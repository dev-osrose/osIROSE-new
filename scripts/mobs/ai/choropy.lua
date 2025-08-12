registerNpc(12, {
  walk_speed        = 175,
  run_speed         = 260,
  scale             = 45,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 6,
  hp                = 16,
  attack            = 10,
  hit               = 61,
  def               = 33,
  res               = 16,
  avoid             = 8,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 11,
  drop_type         = 105,
  drop_money        = 8,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(903, {
  walk_speed        = 160,
  run_speed         = 240,
  scale             = 45,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 27,
  hp                = 1058,
  attack            = 81,
  hit               = 80,
  def               = 100,
  res               = 46,
  avoid             = 35,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 165,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 27,
  sell_tab0         = 27,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
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