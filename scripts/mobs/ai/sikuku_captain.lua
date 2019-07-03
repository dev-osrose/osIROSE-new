registerNpc(544, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 120,
  r_weapon          = 1109,
  l_weapon          = 0,
  level             = 173,
  hp                = 43,
  attack            = 815,
  hit               = 516,
  def               = 559,
  res               = 475,
  avoid             = 283,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 297,
  give_exp          = 162,
  drop_type         = 463,
  drop_money        = 27,
  drop_item         = 44,
  union_number      = 44,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 270,
  npc_type          = 3,
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