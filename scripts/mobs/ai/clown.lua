registerNpc(182, {
  walk_speed        = 0,
  run_speed         = 165,
  scale             = 700,
  r_weapon          = 110,
  l_weapon          = 0,
  level             = 0,
  hp                = 49,
  attack            = 28,
  hit               = 219,
  def               = 127,
  res               = 140,
  avoid             = 93,
  attack_spd        = 75,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 5,
  drop_type         = 52,
  drop_money        = 192,
  drop_item         = 23,
  union_number      = 23,
  need_summon_count = 55,
  sell_tab0         = 55,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 160,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 13,
  height            = 0
});
registerNpc(926, {
  walk_speed        = 0,
  run_speed         = 260,
  scale             = 550,
  r_weapon          = 130,
  l_weapon          = 0,
  level             = 0,
  hp                = 59,
  attack            = 2304,
  hit               = 195,
  def               = 143,
  res               = 189,
  avoid             = 75,
  attack_spd        = 72,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 32,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
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