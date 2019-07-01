registerNpc(206, {
  walk_speed        = 200,
  run_speed         = 550,
  scale             = 180,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 43,
  hp                = 35,
  attack            = 209,
  hit               = 131,
  def               = 132,
  res               = 62,
  avoid             = 72,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 8,
  give_exp          = 59,
  drop_type         = 201,
  drop_money        = 1,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
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