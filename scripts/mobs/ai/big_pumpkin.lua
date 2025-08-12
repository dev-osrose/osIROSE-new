registerNpc(22, {
  walk_speed        = 200,
  run_speed         = 260,
  scale             = 60,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 10,
  hp                = 35,
  attack            = 47,
  hit               = 74,
  def               = 31,
  res               = 27,
  avoid             = 10,
  attack_spd        = 85,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 22,
  drop_type         = 109,
  drop_money        = 10,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 2,
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