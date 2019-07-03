registerNpc(213, {
  walk_speed        = 300,
  run_speed         = 700,
  scale             = 300,
  r_weapon          = 167,
  l_weapon          = 0,
  level             = 42,
  hp                = 161,
  attack            = 230,
  hit               = 154,
  def               = 162,
  res               = 112,
  avoid             = 51,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 318,
  give_exp          = 619,
  drop_type         = 202,
  drop_money        = 10,
  drop_item         = 85,
  union_number      = 85,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
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