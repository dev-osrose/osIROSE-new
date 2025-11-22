registerNpc(132, {
  walk_speed        = 180,
  run_speed         = 680,
  scale             = 105,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 33,
  hp                = 26,
  attack            = 148,
  hit               = 101,
  def               = 95,
  res               = 60,
  avoid             = 54,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 3,
  give_exp          = 43,
  drop_type         = 158,
  drop_money        = 25,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(917, {
  walk_speed        = 170,
  run_speed         = 390,
  scale             = 100,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 50,
  hp                = 1979,
  attack            = 158,
  hit               = 121,
  def               = 174,
  res               = 76,
  avoid             = 57,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 31,
  sell_tab0         = 31,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 0,
  hit_material_type = 1,
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