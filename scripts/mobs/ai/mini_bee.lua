registerNpc(301, {
  walk_speed        = 280,
  run_speed         = 400,
  scale             = 30,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 9,
  hp                = 17,
  attack            = 10,
  hit               = 34,
  def               = 45,
  res               = 24,
  avoid             = 60,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 92,
  give_exp          = 18,
  drop_type         = 8,
  drop_money        = 1,
  drop_item         = 15,
  union_number      = 15,
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

registerNpc(302, {
  walk_speed        = 280,
  run_speed         = 400,
  scale             = 40,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 13,
  hp                = 16,
  attack            = 20,
  hit               = 70,
  def               = 51,
  res               = 28,
  avoid             = 90,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 3,
  give_exp          = 30,
  drop_type         = 8,
  drop_money        = 1,
  drop_item         = 20,
  union_number      = 20,
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