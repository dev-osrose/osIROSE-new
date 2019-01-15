registerNpc(342, {
  walk_speed        = 0,
  run_speed         = 210,
  scale             = 700,
  r_weapon          = 160,
  l_weapon          = 1017,
  level             = 0,
  hp                = 93,
  attack            = 27,
  hit               = 438,
  def               = 230,
  res               = 363,
  avoid             = 154,
  attack_spd        = 151,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 125,
  drop_type         = 80,
  drop_money        = 373,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 50,
  sell_tab0         = 50,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1800,
  hit_material_type = 6,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 16,
  height            = 0
});
registerNpc(934, {
  walk_speed        = 0,
  run_speed         = 210,
  scale             = 530,
  r_weapon          = 175,
  l_weapon          = 1017,
  level             = 0,
  hp                = 94,
  attack            = 4089,
  hit               = 374,
  def               = 265,
  res               = 311,
  avoid             = 158,
  attack_spd        = 140,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 168,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 38,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1600,
  hit_material_type = 0,
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