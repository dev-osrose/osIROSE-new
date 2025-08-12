registerNpc(366, {
  walk_speed        = 280,
  run_speed         = 850,
  scale             = 200,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 86,
  hp                = 40,
  attack            = 429,
  hit               = 215,
  def               = 330,
  res               = 143,
  avoid             = 136,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 144,
  give_exp          = 120,
  drop_type         = 365,
  drop_money        = 10,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 8,
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