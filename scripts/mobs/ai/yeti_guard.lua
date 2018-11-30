registerNpc(343, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 750,
  r_weapon          = 165,
  l_weapon          = 1008,
  level             = 304,
  hp                = 94,
  attack            = 40,
  hit               = 395,
  def               = 227,
  res               = 491,
  avoid             = 229,
  attack_spd        = 81,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 126,
  drop_type         = 88,
  drop_money        = 375,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 50,
  sell_tab0         = 50,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 16,
  height            = 0
});
registerNpc(935, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 560,
  r_weapon          = 190,
  l_weapon          = 1008,
  level             = 304,
  hp                = 97,
  attack            = 4385,
  hit               = 382,
  def               = 252,
  res               = 383,
  avoid             = 164,
  attack_spd        = 130,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 169,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 41,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
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