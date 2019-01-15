registerNpc(12, {
  walk_speed        = 0,
  run_speed         = 175,
  scale             = 260,
  r_weapon          = 45,
  l_weapon          = 0,
  level             = 0,
  hp                = 6,
  attack            = 16,
  hit               = 10,
  def               = 61,
  res               = 33,
  avoid             = 16,
  attack_spd        = 8,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 2,
  drop_type         = 11,
  drop_money        = 105,
  drop_item         = 8,
  union_number      = 8,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
  height            = 0
});
registerNpc(903, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 240,
  r_weapon          = 45,
  l_weapon          = 0,
  level             = 0,
  hp                = 27,
  attack            = 1058,
  hit               = 81,
  def               = 80,
  res               = 100,
  avoid             = 46,
  attack_spd        = 35,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 165,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 27,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
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