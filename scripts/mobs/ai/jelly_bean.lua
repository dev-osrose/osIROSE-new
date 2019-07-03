registerNpc(2, {
  walk_speed        = 170,
  run_speed         = 390,
  scale             = 62,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 4,
  hp                = 15,
  attack            = 4,
  hit               = 59,
  def               = 29,
  res               = 13,
  avoid             = 7,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 102,
  give_exp          = 9,
  drop_type         = 101,
  drop_money        = 8,
  drop_item         = 72,
  union_number      = 72,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 210,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(901, {
  walk_speed        = 170,
  run_speed         = 390,
  scale             = 65,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 26,
  hp                = 980,
  attack            = 80,
  hit               = 81,
  def               = 87,
  res               = 36,
  avoid             = 36,
  attack_spd        = 80,
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