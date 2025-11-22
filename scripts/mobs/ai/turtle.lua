registerNpc(84, {
  walk_speed        = 160,
  run_speed         = 370,
  scale             = 75,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 27,
  hp                = 25,
  attack            = 122,
  hit               = 92,
  def               = 80,
  res               = 49,
  avoid             = 46,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 3,
  give_exp          = 33,
  drop_type         = 138,
  drop_money        = 20,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 1,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(911, {
  walk_speed        = 180,
  run_speed         = 410,
  scale             = 100,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 45,
  hp                = 1710,
  attack            = 144,
  hit               = 116,
  def               = 142,
  res               = 58,
  avoid             = 56,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 166,
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