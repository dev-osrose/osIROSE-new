registerNpc(21, {
  walk_speed        = 200,
  run_speed         = 260,
  scale             = 45,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 9,
  hp                = 23,
  attack            = 47,
  hit               = 65,
  def               = 18,
  res               = 20,
  avoid             = 25,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 18,
  drop_type         = 108,
  drop_money        = 10,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(904, {
  walk_speed        = 200,
  run_speed         = 260,
  scale             = 60,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 30,
  hp                = 918,
  attack            = 90,
  hit               = 86,
  def               = 80,
  res               = 41,
  avoid             = 83,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 165,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 28,
  sell_tab0         = 28,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
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