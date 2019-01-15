registerNpc(121, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 700,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 64,
  attack            = 30,
  hit               = 288,
  def               = 154,
  res               = 187,
  avoid             = 128,
  attack_spd        = 96,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 114,
  drop_type         = 54,
  drop_money        = 154,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 55,
  sell_tab0         = 55,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
  height            = 0
});
registerNpc(916, {
  walk_speed        = 0,
  run_speed         = 195,
  scale             = 440,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 74,
  attack            = 2745,
  hit               = 265,
  def               = 193,
  res               = 230,
  avoid             = 82,
  attack_spd        = 84,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 167,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 35,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 0,
  face_icon         = 2,
  summon_mob_type   = 2,
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