registerNpc(72, {
  walk_speed        = 140,
  run_speed         = 700,
  scale             = 88,
  r_weapon          = 1011,
  l_weapon          = 0,
  level             = 13,
  hp                = 25,
  attack            = 64,
  hit               = 71,
  def               = 47,
  res               = 26,
  avoid             = 30,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 26,
  drop_type         = 132,
  drop_money        = 20,
  drop_item         = 65,
  union_number      = 65,
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

registerNpc(908, {
  walk_speed        = 170,
  run_speed         = 390,
  scale             = 95,
  r_weapon          = 1011,
  l_weapon          = 0,
  level             = 35,
  hp                = 1315,
  attack            = 110,
  hit               = 97,
  def               = 112,
  res               = 46,
  avoid             = 45,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 165,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 29,
  sell_tab0         = 29,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
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