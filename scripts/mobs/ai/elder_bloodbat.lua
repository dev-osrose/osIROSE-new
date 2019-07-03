registerNpc(258, {
  walk_speed        = 500,
  run_speed         = 850,
  scale             = 115,
  r_weapon          = 1076,
  l_weapon          = 0,
  level             = 60,
  hp                = 32,
  attack            = 281,
  hit               = 137,
  def               = 150,
  res               = 122,
  avoid             = 90,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 7,
  give_exp          = 43,
  drop_type         = 223,
  drop_money        = 1,
  drop_item         = 48,
  union_number      = 48,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
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