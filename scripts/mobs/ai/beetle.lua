registerNpc(81, {
  walk_speed        = 0,
  run_speed         = 150,
  scale             = 420,
  r_weapon          = 70,
  l_weapon          = 0,
  level             = 0,
  hp                = 20,
  attack            = 25,
  hit               = 93,
  def               = 81,
  res               = 63,
  avoid             = 37,
  attack_spd        = 38,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 3,
  drop_type         = 30,
  drop_money        = 136,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 58,
  sell_tab0         = 58,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 180,
  hit_material_type = 1,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 11,
  height            = 0
});
registerNpc(910, {
  walk_speed        = 0,
  run_speed         = 190,
  scale             = 380,
  r_weapon          = 80,
  l_weapon          = 0,
  level             = 0,
  hp                = 41,
  attack            = 1603,
  hit               = 127,
  def               = 105,
  res               = 143,
  avoid             = 63,
  attack_spd        = 48,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 165,
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