registerNpc(171, {
  walk_speed        = 180,
  run_speed         = 700,
  scale             = 110,
  r_weapon          = 1035,
  l_weapon          = 0,
  level             = 30,
  hp                = 32,
  attack            = 123,
  hit               = 105,
  def               = 120,
  res               = 64,
  avoid             = 24,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 319,
  give_exp          = 36,
  drop_type         = 186,
  drop_money        = 10,
  drop_item         = 61,
  union_number      = 61,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 2,
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