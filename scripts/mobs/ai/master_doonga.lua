registerNpc(159, {
  walk_speed        = 260,
  run_speed         = 700,
  scale             = 250,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 60,
  hp                = 175,
  attack            = 330,
  hit               = 196,
  def               = 227,
  res               = 161,
  avoid             = 73,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 60,
  give_exp          = 557,
  drop_type         = 178,
  drop_money        = 20,
  drop_item         = 72,
  union_number      = 72,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
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