registerNpc(91, {
  walk_speed        = 130,
  run_speed         = 400,
  scale             = 90,
  r_weapon          = 1011,
  l_weapon          = 0,
  level             = 24,
  hp                = 25,
  attack            = 109,
  hit               = 87,
  def               = 73,
  res               = 44,
  avoid             = 43,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 106,
  give_exp          = 31,
  drop_type         = 142,
  drop_money        = 25,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 1,
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