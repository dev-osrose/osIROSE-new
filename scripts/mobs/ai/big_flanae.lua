registerNpc(45, {
  walk_speed        = 0,
  run_speed         = 0,
  scale             = 200,
  r_weapon          = 1073,
  l_weapon          = 0,
  level             = 21,
  hp                = 31,
  attack            = 88,
  hit               = 91,
  def               = 92,
  res               = 46,
  avoid             = 17,
  attack_spd        = 75,
  is_magic_damage   = 1,
  ai_type           = 18,
  give_exp          = 19,
  drop_type         = 120,
  drop_money        = 1,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2600,
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