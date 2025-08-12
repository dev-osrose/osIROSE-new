registerNpc(73, {
  walk_speed        = 170,
  run_speed         = 700,
  scale             = 90,
  r_weapon          = 1012,
  l_weapon          = 0,
  level             = 15,
  hp                = 28,
  attack            = 75,
  hit               = 68,
  def               = 43,
  res               = 31,
  avoid             = 32,
  attack_spd        = 80,
  is_magic_damage   = 0,
  ai_type           = 3,
  give_exp          = 26,
  drop_type         = 133,
  drop_money        = 20,
  drop_item         = 68,
  union_number      = 68,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 3,
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