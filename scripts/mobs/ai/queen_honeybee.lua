registerNpc(67, {
  walk_speed        = 240,
  run_speed         = 320,
  scale             = 95,
  r_weapon          = 1061,
  l_weapon          = 0,
  level             = 26,
  hp                = 20,
  attack            = 107,
  hit               = 90,
  def               = 51,
  res               = 100,
  avoid             = 45,
  attack_spd        = 80,
  is_magic_damage   = 1,
  ai_type           = 28,
  give_exp          = 30,
  drop_type         = 128,
  drop_money        = 10,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 800,
  npc_type          = 7,
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