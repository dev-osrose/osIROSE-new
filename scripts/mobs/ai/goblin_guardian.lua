registerNpc(219, {
  walk_speed        = 310,
  run_speed         = 730,
  scale             = 330,
  r_weapon          = 173,
  l_weapon          = 0,
  level             = 105,
  hp                = 426,
  attack            = 599,
  hit               = 410,
  def               = 563,
  res               = 289,
  avoid             = 185,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 260,
  give_exp          = 2217,
  drop_type         = 275,
  drop_money        = 0,
  drop_item         = 85,
  union_number      = 85,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
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