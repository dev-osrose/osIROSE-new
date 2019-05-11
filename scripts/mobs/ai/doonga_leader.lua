registerNpc(157, {
  walk_speed        = 250,
  run_speed         = 700,
  scale             = 155,
  r_weapon          = 1021,
  l_weapon          = 0,
  level             = 70,
  hp                = 37,
  attack            = 345,
  hit               = 183,
  def               = 218,
  res               = 110,
  avoid             = 111,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 112,
  give_exp          = 80,
  drop_type         = 176,
  drop_money        = 23,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 290,
  npc_type          = 8,
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