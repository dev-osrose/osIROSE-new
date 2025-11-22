registerNpc(758, {
  walk_speed        = 140,
  run_speed         = 700,
  scale             = 88,
  r_weapon          = 1011,
  l_weapon          = 0,
  level             = 15,
  hp                = 39,
  attack            = 72,
  hit               = 74,
  def               = 52,
  res               = 29,
  avoid             = 32,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 35,
  drop_type         = 0,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
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