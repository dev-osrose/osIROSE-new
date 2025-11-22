registerNpc(134, {
  walk_speed        = 190,
  run_speed         = 700,
  scale             = 110,
  r_weapon          = 133,
  l_weapon          = 0,
  level             = 42,
  hp                = 33,
  attack            = 170,
  hit               = 126,
  def               = 159,
  res               = 89,
  avoid             = 33,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 13,
  give_exp          = 41,
  drop_type         = 160,
  drop_money        = 25,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 2,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(918, {
  walk_speed        = 185,
  run_speed         = 420,
  scale             = 135,
  r_weapon          = 131,
  l_weapon          = 0,
  level             = 56,
  hp                = 1992,
  attack            = 194,
  hit               = 155,
  def               = 167,
  res               = 60,
  avoid             = 64,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 32,
  sell_tab0         = 32,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 0,
  hit_material_type = 0,
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