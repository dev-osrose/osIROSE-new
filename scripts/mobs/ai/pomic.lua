registerNpc(72, {
  walk_speed        = 0,
  run_speed         = 140,
  scale             = 700,
  r_weapon          = 88,
  l_weapon          = 1011,
  level             = 0,
  hp                = 13,
  attack            = 25,
  hit               = 64,
  def               = 71,
  res               = 47,
  avoid             = 26,
  attack_spd        = 30,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 2,
  drop_type         = 26,
  drop_money        = 132,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 65,
  sell_tab0         = 65,
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
registerNpc(908, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 390,
  r_weapon          = 95,
  l_weapon          = 1011,
  level             = 0,
  hp                = 35,
  attack            = 1315,
  hit               = 110,
  def               = 97,
  res               = 112,
  avoid             = 46,
  attack_spd        = 45,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 165,
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
  npc_type          = 300,
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