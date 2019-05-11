registerNpc(257, {
  walk_speed        = 480,
  run_speed         = 800,
  scale             = 105,
  r_weapon          = 1076,
  l_weapon          = 0,
  level             = 50,
  hp                = 27,
  attack            = 208,
  hit               = 125,
  def               = 106,
  res               = 111,
  avoid             = 96,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 7,
  give_exp          = 36,
  drop_type         = 222,
  drop_money        = 1,
  drop_item         = 52,
  union_number      = 52,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
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