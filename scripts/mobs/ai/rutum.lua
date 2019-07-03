registerNpc(382, {
  walk_speed        = 300,
  run_speed         = 1100,
  scale             = 280,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 110,
  hp                = 210,
  attack            = 631,
  hit               = 310,
  def               = 780,
  res               = 327,
  avoid             = 141,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 337,
  give_exp          = 914,
  drop_type         = 390,
  drop_money        = 60,
  drop_item         = 80,
  union_number      = 80,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 11,
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