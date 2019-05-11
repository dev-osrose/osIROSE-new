registerNpc(256, {
  walk_speed        = 450,
  run_speed         = 750,
  scale             = 90,
  r_weapon          = 1076,
  l_weapon          = 0,
  level             = 46,
  hp                = 27,
  attack            = 205,
  hit               = 122,
  def               = 132,
  res               = 87,
  avoid             = 71,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 7,
  give_exp          = 35,
  drop_type         = 221,
  drop_money        = 1,
  drop_item         = 55,
  union_number      = 55,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 180,
  npc_type          = 1,
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