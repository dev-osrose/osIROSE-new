registerNpc(277, {
  walk_speed        = 225,
  run_speed         = 850,
  scale             = 155,
  r_weapon          = 40,
  l_weapon          = 3,
  level             = 85,
  hp                = 39,
  attack            = 354,
  hit               = 208,
  def               = 389,
  res               = 202,
  avoid             = 72,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 116,
  give_exp          = 54,
  drop_type         = 232,
  drop_money        = 15,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 2,
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