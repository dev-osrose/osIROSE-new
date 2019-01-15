registerNpc(114, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 500,
  r_weapon          = 116,
  l_weapon          = 1016,
  level             = 0,
  hp                = 42,
  attack            = 44,
  hit               = 215,
  def               = 133,
  res               = 135,
  avoid             = 86,
  attack_spd        = 51,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 24,
  drop_type         = 80,
  drop_money        = 153,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 65,
  sell_tab0         = 65,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 9,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
  height            = 0
});
registerNpc(180, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 700,
  r_weapon          = 140,
  l_weapon          = 1016,
  level             = 0,
  hp                = 45,
  attack            = 66,
  hit               = 231,
  def               = 152,
  res               = 144,
  avoid             = 92,
  attack_spd        = 55,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 262,
  drop_type         = 90,
  drop_money        = 153,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 39,
  sell_tab0         = 39,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 9,
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