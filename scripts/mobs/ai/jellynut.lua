registerNpc(3, {
  walk_speed        = 175,
  run_speed         = 400,
  scale             = 70,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 5,
  hp                = 15,
  attack            = 8,
  hit               = 60,
  def               = 31,
  res               = 15,
  avoid             = 9,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 103,
  give_exp          = 11,
  drop_type         = 102,
  drop_money        = 12,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(902, {
  walk_speed        = 180,
  run_speed         = 410,
  scale             = 80,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 28,
  hp                = 960,
  attack            = 94,
  hit               = 100,
  def               = 86,
  res               = 32,
  avoid             = 36,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 165,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 28,
  sell_tab0         = 28,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
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