registerNpc(548, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 110,
  r_weapon          = 1111,
  l_weapon          = 0,
  level             = 155,
  hp                = 35,
  attack            = 773,
  hit               = 524,
  def               = 399,
  res               = 443,
  avoid             = 301,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 291,
  give_exp          = 148,
  drop_type         = 467,
  drop_money        = 30,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
  npc_type          = 5,
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