registerNpc(74, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 700,
  r_weapon          = 92,
  l_weapon          = 1031,
  level             = 0,
  hp                = 17,
  attack            = 32,
  hit               = 73,
  def               = 84,
  res               = 81,
  avoid             = 39,
  attack_spd        = 15,
  is_magic_damage   = 75,
  ai_type           = 0,
  give_exp          = 2,
  drop_type         = 29,
  drop_money        = 134,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 11,
  height            = 0
});
registerNpc(909, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 410,
  r_weapon          = 100,
  l_weapon          = 1031,
  level             = 0,
  hp                = 36,
  attack            = 1236,
  hit               = 121,
  def               = 115,
  res               = 107,
  avoid             = 39,
  attack_spd        = 43,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 29,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
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