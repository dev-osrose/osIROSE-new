registerNpc(21, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 260,
  r_weapon          = 45,
  l_weapon          = 0,
  level             = 0,
  hp                = 9,
  attack            = 23,
  hit               = 47,
  def               = 65,
  res               = 18,
  avoid             = 20,
  attack_spd        = 25,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 2,
  drop_type         = 18,
  drop_money        = 108,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 58,
  sell_tab0         = 58,
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
registerNpc(904, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 260,
  r_weapon          = 60,
  l_weapon          = 0,
  level             = 0,
  hp                = 30,
  attack            = 918,
  hit               = 90,
  def               = 86,
  res               = 80,
  avoid             = 41,
  attack_spd        = 83,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 165,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 28,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
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