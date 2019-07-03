registerNpc(87, {
  walk_speed        = 130,
  run_speed         = 400,
  scale             = 66,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 20,
  hp                = 25,
  attack            = 93,
  hit               = 81,
  def               = 63,
  res               = 37,
  avoid             = 38,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 21,
  give_exp          = 30,
  drop_type         = 140,
  drop_money        = 8,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 1,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(912, {
  walk_speed        = 170,
  run_speed         = 390,
  scale             = 80,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 39,
  hp                = 1470,
  attack            = 123,
  hit               = 104,
  def               = 124,
  res               = 51,
  avoid             = 50,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 29,
  sell_tab0         = 29,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 0,
  hit_material_type = 2,
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