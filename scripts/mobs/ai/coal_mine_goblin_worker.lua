registerNpc(272, {
  walk_speed        = 220,
  run_speed         = 850,
  scale             = 120,
  r_weapon          = 1015,
  l_weapon          = 0,
  level             = 55,
  hp                = 35,
  attack            = 224,
  hit               = 149,
  def               = 205,
  res               = 120,
  avoid             = 44,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 328,
  give_exp          = 39,
  drop_type         = 227,
  drop_money        = 12,
  drop_item         = 55,
  union_number      = 55,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 2,
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