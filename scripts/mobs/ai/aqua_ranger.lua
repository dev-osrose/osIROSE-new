registerNpc(175, {
  walk_speed        = 240,
  run_speed         = 700,
  scale             = 112,
  r_weapon          = 1043,
  l_weapon          = 0,
  level             = 32,
  hp                = 24,
  attack            = 134,
  hit               = 96,
  def               = 66,
  res               = 70,
  avoid             = 68,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 321,
  give_exp          = 38,
  drop_type         = 189,
  drop_money        = 12,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2200,
  npc_type          = 5,
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