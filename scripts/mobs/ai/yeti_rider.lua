registerNpc(351, {
  walk_speed        = 280,
  run_speed         = 800,
  scale             = 190,
  r_weapon          = 1036,
  l_weapon          = 0,
  level             = 105,
  hp                = 35,
  attack            = 490,
  hit               = 233,
  def               = 582,
  res               = 240,
  avoid             = 160,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 131,
  give_exp          = 112,
  drop_type         = 384,
  drop_money        = 0,
  drop_item         = 48,
  union_number      = 48,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(936, {
  walk_speed        = 280,
  run_speed         = 650,
  scale             = 200,
  r_weapon          = 1036,
  l_weapon          = 0,
  level             = 100,
  hp                = 4409,
  attack            = 401,
  hit               = 260,
  def               = 367,
  res               = 170,
  avoid             = 135,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 168,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 45,
  sell_tab0         = 45,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
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