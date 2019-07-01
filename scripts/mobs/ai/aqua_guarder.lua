registerNpc(925, {
  walk_speed        = 180,
  run_speed         = 410,
  scale             = 115,
  r_weapon          = 1035,
  l_weapon          = 0,
  level             = 48,
  hp                = 1894,
  attack            = 151,
  hit               = 118,
  def               = 167,
  res               = 73,
  avoid             = 55,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 167,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 31,
  sell_tab0         = 31,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 0,
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