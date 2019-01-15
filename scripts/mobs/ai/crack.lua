registerNpc(87, {
  walk_speed        = 0,
  run_speed         = 130,
  scale             = 400,
  r_weapon          = 66,
  l_weapon          = 0,
  level             = 0,
  hp                = 20,
  attack            = 25,
  hit               = 93,
  def               = 81,
  res               = 63,
  avoid             = 37,
  attack_spd        = 38,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 21,
  drop_type         = 30,
  drop_money        = 140,
  drop_item         = 8,
  union_number      = 8,
  need_summon_count = 50,
  sell_tab0         = 50,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 1,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 16,
  height            = 0
});
registerNpc(912, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 390,
  r_weapon          = 80,
  l_weapon          = 0,
  level             = 0,
  hp                = 39,
  attack            = 1470,
  hit               = 123,
  def               = 104,
  res               = 124,
  avoid             = 51,
  attack_spd        = 50,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 166,
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
  npc_type          = 250,
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