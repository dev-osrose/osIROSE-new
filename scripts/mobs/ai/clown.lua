registerNpc(182, {
  walk_speed        = 165,
  run_speed         = 700,
  scale             = 110,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 49,
  hp                = 28,
  attack            = 219,
  hit               = 127,
  def               = 140,
  res               = 93,
  avoid             = 75,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 5,
  give_exp          = 52,
  drop_type         = 192,
  drop_money        = 23,
  drop_item         = 55,
  union_number      = 55,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 160,
  npc_type          = 1,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(926, {
  walk_speed        = 260,
  run_speed         = 550,
  scale             = 130,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 59,
  hp                = 2304,
  attack            = 195,
  hit               = 143,
  def               = 189,
  res               = 75,
  avoid             = 72,
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