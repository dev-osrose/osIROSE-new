registerNpc(102, {
  walk_speed        = 190,
  run_speed         = 480,
  scale             = 96,
  r_weapon          = 1080,
  l_weapon          = 0,
  level             = 17,
  hp                = 23,
  attack            = 75,
  hit               = 74,
  def               = 37,
  res               = 40,
  avoid             = 46,
  attack_spd        = 85,
  is_magic_damage   = 0,
  ai_type           = 26,
  give_exp          = 28,
  drop_type         = 148,
  drop_money        = 30,
  drop_item         = 63,
  union_number      = 63,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
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