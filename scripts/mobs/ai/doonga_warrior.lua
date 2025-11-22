registerNpc(155, {
  walk_speed        = 200,
  run_speed         = 700,
  scale             = 145,
  r_weapon          = 1004,
  l_weapon          = 303,
  level             = 64,
  hp                = 30,
  attack            = 318,
  hit               = 175,
  def               = 201,
  res               = 94,
  avoid             = 66,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 320,
  give_exp          = 51,
  drop_type         = 174,
  drop_money        = 15,
  drop_item         = 54,
  union_number      = 54,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 4,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(922, {
  walk_speed        = 200,
  run_speed         = 450,
  scale             = 150,
  r_weapon          = 1004,
  l_weapon          = 0,
  level             = 72,
  hp                = 2981,
  attack            = 238,
  hit               = 165,
  def               = 257,
  res               = 108,
  avoid             = 81,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 167,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 34,
  sell_tab0         = 34,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 0,
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