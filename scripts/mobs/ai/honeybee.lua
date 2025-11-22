registerNpc(62, {
  walk_speed        = 220,
  run_speed         = 440,
  scale             = 70,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 12,
  hp                = 25,
  attack            = 60,
  hit               = 70,
  def               = 45,
  res               = 24,
  avoid             = 29,
  attack_spd        = 93,
  is_magic_damage   = 0,
  ai_type           = 3,
  give_exp          = 27,
  drop_type         = 125,
  drop_money        = 10,
  drop_item         = 60,
  union_number      = 60,
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

registerNpc(906, {
  walk_speed        = 220,
  run_speed         = 300,
  scale             = 65,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 32,
  hp                = 1201,
  attack            = 100,
  hit               = 92,
  def               = 104,
  res               = 43,
  avoid             = 42,
  attack_spd        = 80,
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