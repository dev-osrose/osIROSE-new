registerNpc(207, {
  walk_speed        = 290,
  run_speed         = 850,
  scale             = 220,
  r_weapon          = 167,
  l_weapon          = 0,
  level             = 40,
  hp                = 160,
  attack            = 219,
  hit               = 150,
  def               = 155,
  res               = 106,
  avoid             = 49,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 324,
  give_exp          = 607,
  drop_type         = 202,
  drop_money        = 15,
  drop_item         = 80,
  union_number      = 80,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 10,
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