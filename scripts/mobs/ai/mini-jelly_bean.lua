registerNpc(1, {
  walk_speed        = 160,
  run_speed         = 370,
  scale             = 56,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 2,
  hp                = 15,
  attack            = 2,
  hit               = 56,
  def               = 25,
  res               = 11,
  avoid             = 5,
  attack_spd        = 80,
  is_magic_damage   = 0,
  ai_type           = 101,
  give_exp          = 6,
  drop_type         = 100,
  drop_money        = 6,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 190,
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