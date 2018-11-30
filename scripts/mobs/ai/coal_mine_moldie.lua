registerNpc(112, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 460,
  r_weapon          = 105,
  l_weapon          = 0,
  level             = 0,
  hp                = 38,
  attack            = 32,
  hit               = 154,
  def               = 119,
  res               = 145,
  avoid             = 81,
  attack_spd        = 30,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 107,
  drop_type         = 45,
  drop_money        = 151,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 2,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
  height            = 0
});
registerNpc(179, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 700,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 42,
  attack            = 49,
  hit               = 170,
  def               = 133,
  res               = 159,
  avoid             = 89,
  attack_spd        = 33,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 267,
  drop_type         = 45,
  drop_money        = 152,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 32,
  sell_tab0         = 32,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 2,
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