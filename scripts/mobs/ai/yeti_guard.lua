registerNpc(343, {
  walk_speed        = 225,
  run_speed         = 750,
  scale             = 165,
  r_weapon          = 1008,
  l_weapon          = 304,
  level             = 94,
  hp                = 40,
  attack            = 395,
  hit               = 227,
  def               = 491,
  res               = 229,
  avoid             = 81,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 126,
  give_exp          = 88,
  drop_type         = 375,
  drop_money        = 0,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 2,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(935, {
  walk_speed        = 225,
  run_speed         = 560,
  scale             = 190,
  r_weapon          = 1008,
  l_weapon          = 304,
  level             = 97,
  hp                = 4385,
  attack            = 382,
  hit               = 252,
  def               = 383,
  res               = 164,
  avoid             = 130,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 169,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 41,
  sell_tab0         = 41,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
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