registerNpc(68, {
  walk_speed        = 240,
  run_speed         = 450,
  scale             = 107,
  r_weapon          = 1061,
  l_weapon          = 0,
  level             = 27,
  hp                = 34,
  attack            = 133,
  hit               = 103,
  def               = 88,
  res               = 38,
  avoid             = 51,
  attack_spd        = 95,
  is_magic_damage   = 1,
  ai_type           = 28,
  give_exp          = 56,
  drop_type         = 129,
  drop_money        = 10,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 900,
  npc_type          = 8,
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