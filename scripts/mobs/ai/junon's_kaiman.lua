registerNpc(404, {
  walk_speed        = 230,
  run_speed         = 680,
  scale             = 350,
  r_weapon          = 105,
  l_weapon          = 0,
  level             = 70,
  hp                = 214,
  attack            = 371,
  hit               = 233,
  def               = 255,
  res               = 154,
  avoid             = 109,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 67,
  give_exp          = 658,
  drop_type         = 304,
  drop_money        = 1,
  drop_item         = 90,
  union_number      = 90,
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