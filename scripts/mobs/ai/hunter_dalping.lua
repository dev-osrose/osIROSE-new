registerNpc(33, {
  walk_speed        = 220,
  run_speed         = 314,
  scale             = 100,
  r_weapon          = 1071,
  l_weapon          = 0,
  level             = 24,
  hp                = 20,
  attack            = 112,
  hit               = 105,
  def               = 65,
  res               = 28,
  avoid             = 48,
  attack_spd        = 80,
  is_magic_damage   = 0,
  ai_type           = 28,
  give_exp          = 29,
  drop_type         = 114,
  drop_money        = 9,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1800,
  npc_type          = 6,
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