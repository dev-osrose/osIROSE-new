registerNpc(84, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 370,
  r_weapon          = 75,
  l_weapon          = 0,
  level             = 0,
  hp                = 27,
  attack            = 25,
  hit               = 122,
  def               = 92,
  res               = 80,
  avoid             = 49,
  attack_spd        = 46,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 3,
  drop_type         = 33,
  drop_money        = 138,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 58,
  sell_tab0         = 58,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 1,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 12,
  height            = 0
});
registerNpc(911, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 410,
  r_weapon          = 100,
  l_weapon          = 0,
  level             = 0,
  hp                = 45,
  attack            = 1710,
  hit               = 144,
  def               = 116,
  res               = 142,
  avoid             = 58,
  attack_spd        = 56,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 30,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 0,
  face_icon         = 2,
  summon_mob_type   = 2,
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