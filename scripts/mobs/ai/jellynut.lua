registerNpc(3, {
  walk_speed        = 0,
  run_speed         = 175,
  scale             = 400,
  r_weapon          = 70,
  l_weapon          = 0,
  level             = 0,
  hp                = 5,
  attack            = 15,
  hit               = 8,
  def               = 60,
  res               = 31,
  avoid             = 15,
  attack_spd        = 9,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 103,
  drop_type         = 11,
  drop_money        = 102,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 70,
  sell_tab0         = 70,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
  height            = 0
});
registerNpc(902, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 410,
  r_weapon          = 80,
  l_weapon          = 0,
  level             = 0,
  hp                = 28,
  attack            = 960,
  hit               = 94,
  def               = 100,
  res               = 86,
  avoid             = 32,
  attack_spd        = 36,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 165,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 28,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
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