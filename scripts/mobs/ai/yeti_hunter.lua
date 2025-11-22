registerNpc(342, {
  walk_speed        = 210,
  run_speed         = 700,
  scale             = 160,
  r_weapon          = 1017,
  l_weapon          = 0,
  level             = 93,
  hp                = 27,
  attack            = 438,
  hit               = 230,
  def               = 363,
  res               = 154,
  avoid             = 151,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 125,
  give_exp          = 80,
  drop_type         = 373,
  drop_money        = 0,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1800,
  npc_type          = 6,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(934, {
  walk_speed        = 210,
  run_speed         = 530,
  scale             = 175,
  r_weapon          = 1017,
  l_weapon          = 0,
  level             = 94,
  hp                = 4089,
  attack            = 374,
  hit               = 265,
  def               = 311,
  res               = 158,
  avoid             = 140,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 168,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 38,
  sell_tab0         = 38,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1600,
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