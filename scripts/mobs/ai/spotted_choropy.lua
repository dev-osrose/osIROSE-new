registerNpc(15, {
  walk_speed        = 180,
  run_speed         = 266,
  scale             = 50,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 11,
  hp                = 24,
  attack            = 56,
  hit               = 68,
  def               = 43,
  res               = 23,
  avoid             = 28,
  attack_spd        = 85,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 26,
  drop_type         = 107,
  drop_money        = 12,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
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