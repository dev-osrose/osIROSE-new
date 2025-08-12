registerNpc(135, {
  walk_speed        = 180,
  run_speed         = 700,
  scale             = 120,
  r_weapon          = 134,
  l_weapon          = 0,
  level             = 44,
  hp                = 30,
  attack            = 205,
  hit               = 111,
  def               = 108,
  res               = 86,
  avoid             = 68,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 115,
  give_exp          = 43,
  drop_type         = 161,
  drop_money        = 25,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 3,
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