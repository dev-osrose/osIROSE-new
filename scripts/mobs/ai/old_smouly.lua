registerNpc(53, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 338,
  r_weapon          = 135,
  l_weapon          = 1075,
  level             = 0,
  hp                = 48,
  attack            = 35,
  hit               = 234,
  def               = 140,
  res               = 147,
  avoid             = 70,
  attack_spd        = 79,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 29,
  drop_type         = 60,
  drop_money        = 123,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1300,
  hit_material_type = 8,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
  height            = 0
});
registerNpc(188, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 700,
  r_weapon          = 140,
  l_weapon          = 1075,
  level             = 0,
  hp                = 56,
  attack            = 54,
  hit               = 273,
  def               = 159,
  res               = 172,
  avoid             = 84,
  attack_spd        = 90,
  is_magic_damage   = 120,
  ai_type           = 1,
  give_exp          = 264,
  drop_type         = 66,
  drop_money        = 123,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 35,
  sell_tab0         = 35,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1600,
  hit_material_type = 8,
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