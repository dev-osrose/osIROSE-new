registerNpc(629, {
  walk_speed        = 500,
  run_speed         = 1050,
  scale             = 400,
  r_weapon          = 1116,
  l_weapon          = 0,
  level             = 70,
  hp                = 182,
  attack            = 387,
  hit               = 214,
  def               = 266,
  res               = 191,
  avoid             = 86,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 276,
  give_exp          = 800,
  drop_type         = 333,
  drop_money        = 12,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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