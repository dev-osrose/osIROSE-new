registerNpc(331, {
  walk_speed        = 220,
  run_speed         = 506,
  scale             = 85,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 72,
  hp                = 31,
  attack            = 326,
  hit               = 168,
  def               = 214,
  res               = 148,
  avoid             = 108,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 139,
  give_exp          = 61,
  drop_type         = 355,
  drop_money        = 0,
  drop_item         = 46,
  union_number      = 46,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 1,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});

registerNpc(931, {
  walk_speed        = 250,
  run_speed         = 560,
  scale             = 135,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 82,
  hp                = 2768,
  attack            = 321,
  hit               = 191,
  def               = 224,
  res               = 170,
  avoid             = 132,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 166,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 30,
  sell_tab0         = 30,
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