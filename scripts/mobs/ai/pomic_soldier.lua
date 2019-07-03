registerNpc(74, {
  walk_speed        = 160,
  run_speed         = 700,
  scale             = 92,
  r_weapon          = 1031,
  l_weapon          = 0,
  level             = 17,
  hp                = 32,
  attack            = 73,
  hit               = 84,
  def               = 81,
  res               = 39,
  avoid             = 15,
  attack_spd        = 75,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 29,
  drop_type         = 134,
  drop_money        = 30,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 2,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(909, {
  walk_speed        = 180,
  run_speed         = 410,
  scale             = 100,
  r_weapon          = 1031,
  l_weapon          = 0,
  level             = 36,
  hp                = 1236,
  attack            = 121,
  hit               = 115,
  def               = 107,
  res               = 39,
  avoid             = 43,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 166,
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
  attack_range      = 350,
  npc_type          = 0,
  hit_material_type = 0,
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