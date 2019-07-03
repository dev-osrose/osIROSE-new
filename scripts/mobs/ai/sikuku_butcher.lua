registerNpc(545, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 120,
  r_weapon          = 1110,
  l_weapon          = 0,
  level             = 174,
  hp                = 44,
  attack            = 820,
  hit               = 519,
  def               = 564,
  res               = 479,
  avoid             = 285,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 297,
  give_exp          = 171,
  drop_type         = 464,
  drop_money        = 28,
  drop_item         = 45,
  union_number      = 45,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
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