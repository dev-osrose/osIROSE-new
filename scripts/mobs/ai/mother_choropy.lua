registerNpc(13, {
  walk_speed        = 180,
  run_speed         = 266,
  scale             = 60,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 7,
  hp                = 17,
  attack            = 12,
  hit               = 63,
  def               = 41,
  res               = 17,
  avoid             = 8,
  attack_spd        = 80,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 11,
  drop_type         = 106,
  drop_money        = 10,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
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