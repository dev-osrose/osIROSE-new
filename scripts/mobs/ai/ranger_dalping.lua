registerNpc(32, {
  walk_speed        = 210,
  run_speed         = 302,
  scale             = 100,
  r_weapon          = 1071,
  l_weapon          = 0,
  level             = 22,
  hp                = 23,
  attack            = 94,
  hit               = 81,
  def               = 46,
  res               = 50,
  avoid             = 53,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 26,
  give_exp          = 29,
  drop_type         = 113,
  drop_money        = 7,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1800,
  npc_type          = 5,
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