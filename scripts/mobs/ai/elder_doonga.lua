registerNpc(152, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 165,
  r_weapon          = 1013,
  l_weapon          = 0,
  level             = 58,
  hp                = 32,
  attack            = 272,
  hit               = 134,
  def               = 145,
  res               = 118,
  avoid             = 87,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 23,
  give_exp          = 50,
  drop_type         = 171,
  drop_money        = 12,
  drop_item         = 52,
  union_number      = 52,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
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