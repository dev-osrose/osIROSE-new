registerNpc(2, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 390,
  r_weapon          = 62,
  l_weapon          = 0,
  level             = 0,
  hp                = 4,
  attack            = 15,
  hit               = 4,
  def               = 59,
  res               = 29,
  avoid             = 13,
  attack_spd        = 7,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 102,
  drop_type         = 9,
  drop_money        = 101,
  drop_item         = 8,
  union_number      = 8,
  need_summon_count = 72,
  sell_tab0         = 72,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 210,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
  height            = 0
});
registerNpc(901, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 390,
  r_weapon          = 65,
  l_weapon          = 0,
  level             = 0,
  hp                = 26,
  attack            = 980,
  hit               = 80,
  def               = 81,
  res               = 87,
  avoid             = 36,
  attack_spd        = 36,
  is_magic_damage   = 80,
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