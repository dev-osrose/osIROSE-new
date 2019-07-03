registerNpc(536, {
  walk_speed        = 400,
  run_speed         = 850,
  scale             = 120,
  r_weapon          = 1039,
  l_weapon          = 0,
  level             = 140,
  hp                = 34,
  attack            = 677,
  hit               = 412,
  def               = 447,
  res               = 351,
  avoid             = 220,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 292,
  give_exp          = 121,
  drop_type         = 454,
  drop_money        = 30,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 310,
  npc_type          = 1,
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