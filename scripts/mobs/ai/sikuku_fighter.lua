registerNpc(538, {
  walk_speed        = 400,
  run_speed         = 850,
  scale             = 140,
  r_weapon          = 1105,
  l_weapon          = 0,
  level             = 147,
  hp                = 40,
  attack            = 718,
  hit               = 433,
  def               = 476,
  res               = 382,
  avoid             = 233,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 292,
  give_exp          = 148,
  drop_type         = 456,
  drop_money        = 30,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 310,
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