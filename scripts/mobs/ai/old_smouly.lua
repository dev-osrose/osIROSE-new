registerNpc(53, {
  walk_speed        = 240,
  run_speed         = 338,
  scale             = 135,
  r_weapon          = 1075,
  l_weapon          = 0,
  level             = 48,
  hp                = 35,
  attack            = 234,
  hit               = 140,
  def               = 147,
  res               = 70,
  avoid             = 79,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 29,
  give_exp          = 60,
  drop_type         = 123,
  drop_money        = 15,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1300,
  npc_type          = 8,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(188, {
  walk_speed        = 240,
  run_speed         = 700,
  scale             = 140,
  r_weapon          = 1075,
  l_weapon          = 0,
  level             = 56,
  hp                = 54,
  attack            = 273,
  hit               = 159,
  def               = 172,
  res               = 84,
  avoid             = 90,
  attack_spd        = 120,
  is_magic_damage   = 1,
  ai_type           = 264,
  give_exp          = 66,
  drop_type         = 123,
  drop_money        = 15,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1600,
  npc_type          = 8,
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