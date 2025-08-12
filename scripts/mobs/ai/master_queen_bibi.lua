registerNpc(69, {
  walk_speed        = 300,
  run_speed         = 500,
  scale             = 135,
  r_weapon          = 333,
  l_weapon          = 0,
  level             = 33,
  hp                = 43,
  attack            = 170,
  hit               = 117,
  def               = 109,
  res               = 68,
  avoid             = 42,
  attack_spd        = 90,
  is_magic_damage   = 1,
  ai_type           = 28,
  give_exp          = 81,
  drop_type         = 130,
  drop_money        = 12,
  drop_item         = 67,
  union_number      = 67,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
  npc_type          = 9,
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