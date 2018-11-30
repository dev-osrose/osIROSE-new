registerNpc(62, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 440,
  r_weapon          = 70,
  l_weapon          = 0,
  level             = 0,
  hp                = 12,
  attack            = 25,
  hit               = 60,
  def               = 70,
  res               = 45,
  avoid             = 24,
  attack_spd        = 29,
  is_magic_damage   = 93,
  ai_type           = 0,
  give_exp          = 3,
  drop_type         = 27,
  drop_money        = 125,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 60,
  sell_tab0         = 60,
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
registerNpc(906, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 300,
  r_weapon          = 65,
  l_weapon          = 0,
  level             = 0,
  hp                = 32,
  attack            = 1201,
  hit               = 100,
  def               = 92,
  res               = 104,
  avoid             = 43,
  attack_spd        = 42,
  is_magic_damage   = 80,
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