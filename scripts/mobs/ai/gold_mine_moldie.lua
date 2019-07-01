registerNpc(113, {
  walk_speed        = 200,
  run_speed         = 450,
  scale             = 102,
  r_weapon          = 1016,
  l_weapon          = 0,
  level             = 39,
  hp                = 27,
  attack            = 192,
  hit               = 128,
  def               = 122,
  res               = 51,
  avoid             = 40,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 107,
  give_exp          = 36,
  drop_type         = 152,
  drop_money        = 25,
  drop_item         = 52,
  union_number      = 52,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 4,
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