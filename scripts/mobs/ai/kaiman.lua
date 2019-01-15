registerNpc(141, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 700,
  r_weapon          = 140,
  l_weapon          = 103,
  level             = 0,
  hp                = 59,
  attack            = 29,
  hit               = 265,
  def               = 145,
  res               = 171,
  avoid             = 116,
  attack_spd        = 89,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 114,
  drop_type         = 52,
  drop_money        = 165,
  drop_item         = 16,
  union_number      = 16,
  need_summon_count = 48,
  sell_tab0         = 48,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 13,
  height            = 0
});
registerNpc(920, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 450,
  r_weapon          = 170,
  l_weapon          = 104,
  level             = 0,
  hp                = 70,
  attack            = 2572,
  hit               = 249,
  def               = 184,
  res               = 215,
  avoid             = 77,
  attack_spd        = 79,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 167,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 34,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
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