registerNpc(208, {
  walk_speed        = 240,
  run_speed         = 800,
  scale             = 340,
  r_weapon          = 1075,
  l_weapon          = 0,
  level             = 57,
  hp                = 172,
  attack            = 313,
  hit               = 185,
  def               = 216,
  res               = 153,
  avoid             = 69,
  attack_spd        = 90,
  is_magic_damage   = 1,
  ai_type           = 57,
  give_exp          = 519,
  drop_type         = 203,
  drop_money        = 1,
  drop_item         = 75,
  union_number      = 75,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
  npc_type          = 10,
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