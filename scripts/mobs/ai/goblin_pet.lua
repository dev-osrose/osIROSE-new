registerNpc(262, {
  walk_speed        = 160,
  run_speed         = 370,
  scale             = 120,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 50,
  hp                = 34,
  attack            = 203,
  hit               = 140,
  def               = 187,
  res               = 108,
  avoid             = 40,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 1,
  give_exp          = 29,
  drop_type         = 225,
  drop_money        = 75,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 900,
  npc_type          = 2,
  hit_material_type = 2,
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