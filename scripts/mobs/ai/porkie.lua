registerNpc(132, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 680,
  r_weapon          = 105,
  l_weapon          = 0,
  level             = 0,
  hp                = 33,
  attack            = 26,
  hit               = 148,
  def               = 101,
  res               = 95,
  avoid             = 60,
  attack_spd        = 54,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 3,
  drop_type         = 43,
  drop_money        = 158,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
  height            = 0
});
registerNpc(917, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 390,
  r_weapon          = 100,
  l_weapon          = 0,
  level             = 0,
  hp                = 50,
  attack            = 1979,
  hit               = 158,
  def               = 121,
  res               = 174,
  avoid             = 76,
  attack_spd        = 57,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 31,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
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