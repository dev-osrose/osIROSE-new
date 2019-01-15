registerNpc(301, {
  walk_speed        = 0,
  run_speed         = 280,
  scale             = 400,
  r_weapon          = 30,
  l_weapon          = 0,
  level             = 0,
  hp                = 9,
  attack            = 17,
  hit               = 10,
  def               = 34,
  res               = 45,
  avoid             = 24,
  attack_spd        = 60,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 92,
  drop_type         = 18,
  drop_money        = 8,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 15,
  sell_tab0         = 15,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 19,
  height            = 0
});
registerNpc(302, {
  walk_speed        = 0,
  run_speed         = 280,
  scale             = 400,
  r_weapon          = 40,
  l_weapon          = 0,
  level             = 0,
  hp                = 13,
  attack            = 16,
  hit               = 20,
  def               = 70,
  res               = 51,
  avoid             = 28,
  attack_spd        = 90,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 3,
  drop_type         = 30,
  drop_money        = 8,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 20,
  sell_tab0         = 20,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 19,
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