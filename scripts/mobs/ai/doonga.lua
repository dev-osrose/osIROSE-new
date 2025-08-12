registerNpc(151, {
  walk_speed        = 200,
  run_speed         = 700,
  scale             = 140,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 56,
  hp                = 35,
  attack            = 228,
  hit               = 151,
  def               = 209,
  res               = 123,
  avoid             = 45,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 13,
  give_exp          = 48,
  drop_type         = 170,
  drop_money        = 10,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 190,
  npc_type          = 2,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(921, {
  walk_speed        = 200,
  run_speed         = 400,
  scale             = 160,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 67,
  hp                = 2781,
  attack            = 218,
  hit               = 158,
  def               = 210,
  res               = 73,
  avoid             = 69,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 34,
  sell_tab0         = 34,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
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