registerNpc(280, {
  walk_speed        = 230,
  run_speed         = 850,
  scale             = 160,
  r_weapon          = 39,
  l_weapon          = 3,
  level             = 60,
  hp                = 46,
  attack            = 309,
  hit               = 167,
  def               = 193,
  res               = 125,
  avoid             = 73,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 117,
  give_exp          = 75,
  drop_type         = 235,
  drop_money        = 15,
  drop_item         = 57,
  union_number      = 57,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 9,
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