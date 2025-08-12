registerNpc(121, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 120,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 64,
  hp                = 30,
  attack            = 288,
  hit               = 154,
  def               = 187,
  res               = 128,
  avoid             = 96,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 114,
  give_exp          = 54,
  drop_type         = 154,
  drop_money        = 15,
  drop_item         = 55,
  union_number      = 55,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 1,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(916, {
  walk_speed        = 195,
  run_speed         = 440,
  scale             = 120,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 74,
  hp                = 2745,
  attack            = 265,
  hit               = 193,
  def               = 230,
  res               = 82,
  avoid             = 84,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 167,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 35,
  sell_tab0         = 35,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 0,
  hit_material_type = 2,
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