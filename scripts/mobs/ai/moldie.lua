registerNpc(111, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 520,
  r_weapon          = 95,
  l_weapon          = 0,
  level             = 0,
  hp                = 37,
  attack            = 26,
  hit               = 165,
  def               = 107,
  res               = 106,
  avoid             = 68,
  attack_spd        = 59,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 21,
  drop_type         = 47,
  drop_money        = 150,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 58,
  sell_tab0         = 58,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
  height            = 0
});
registerNpc(915, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 410,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 53,
  attack            = 2109,
  hit               = 168,
  def               = 127,
  res               = 184,
  avoid             = 80,
  attack_spd        = 61,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 31,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 0,
  face_icon         = 1,
  summon_mob_type   = 1,
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