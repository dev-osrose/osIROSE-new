registerNpc(88, {
  walk_speed        = 130,
  run_speed         = 320,
  scale             = 75,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 22,
  hp                = 27,
  attack            = 106,
  hit               = 78,
  def               = 57,
  res               = 43,
  avoid             = 40,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 105,
  give_exp          = 30,
  drop_type         = 141,
  drop_money        = 12,
  drop_item         = 55,
  union_number      = 55,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 3,
  hit_material_type = 2,
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