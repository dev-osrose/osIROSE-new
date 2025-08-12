registerNpc(542, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 110,
  r_weapon          = 1108,
  l_weapon          = 0,
  level             = 143,
  hp                = 31,
  attack            = 756,
  hit               = 421,
  def               = 437,
  res               = 368,
  avoid             = 226,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 297,
  give_exp          = 140,
  drop_type         = 461,
  drop_money        = 25,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
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