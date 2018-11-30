registerNpc(161, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 700,
  r_weapon          = 200,
  l_weapon          = 0,
  level             = 0,
  hp                = 47,
  attack            = 74,
  hit               = 253,
  def               = 164,
  res               = 168,
  avoid             = 102,
  attack_spd        = 57,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 9,
  drop_type         = 167,
  drop_money        = 179,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 70,
  sell_tab0         = 70,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 10,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 15,
  height            = 0
});
registerNpc(924, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 500,
  r_weapon          = 260,
  l_weapon          = 0,
  level             = 0,
  hp                = 62,
  attack            = 2593,
  hit               = 222,
  def               = 179,
  res               = 202,
  avoid             = 87,
  attack_spd        = 79,
  is_magic_damage   = 80,
  ai_type           = 0,
  give_exp          = 167,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 33,
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