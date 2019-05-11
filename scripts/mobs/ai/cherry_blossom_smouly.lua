registerNpc(938, {
  walk_speed        = 300,
  run_speed         = 600,
  scale             = 160,
  r_weapon          = 1074,
  l_weapon          = 0,
  level             = 70,
  hp                = 1886,
  attack            = 295,
  hit               = 215,
  def               = 191,
  res               = 90,
  avoid             = 87,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 169,
  give_exp          = 0,
  drop_type         = 3,
  drop_money        = 0,
  drop_item         = 100,
  union_number      = 100,
  need_summon_count = 35,
  sell_tab0         = 35,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1400,
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