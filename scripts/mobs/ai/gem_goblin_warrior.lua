registerNpc(279, {
  walk_speed        = 230,
  run_speed         = 850,
  scale             = 160,
  r_weapon          = 170,
  l_weapon          = 0,
  level             = 89,
  hp                = 33,
  attack            = 451,
  hit               = 227,
  def               = 366,
  res               = 144,
  avoid             = 95,
  attack_spd        = 115,
  is_magic_damage   = 0,
  ai_type           = 117,
  give_exp          = 55,
  drop_type         = 234,
  drop_money        = 15,
  drop_item         = 43,
  union_number      = 43,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 4,
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