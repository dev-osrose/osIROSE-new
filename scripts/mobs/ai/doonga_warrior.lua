registerNpc(155, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 700,
  r_weapon          = 145,
  l_weapon          = 1004,
  level             = 303,
  hp                = 64,
  attack            = 30,
  hit               = 318,
  def               = 175,
  res               = 201,
  avoid             = 94,
  attack_spd        = 66,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 320,
  drop_type         = 51,
  drop_money        = 174,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 54,
  sell_tab0         = 54,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 4,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
  height            = 0
});
registerNpc(922, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 450,
  r_weapon          = 150,
  l_weapon          = 1004,
  level             = 0,
  hp                = 72,
  attack            = 2981,
  hit               = 238,
  def               = 165,
  res               = 257,
  avoid             = 108,
  attack_spd        = 81,
  is_magic_damage   = 95,
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
  face_icon         = 0,
  summon_mob_type   = 0,
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