registerNpc(379, {
  walk_speed        = 300,
  run_speed         = 1050,
  scale             = 350,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 88,
  hp                = 50,
  attack            = 464,
  hit               = 225,
  def               = 353,
  res               = 194,
  avoid             = 110,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 136,
  give_exp          = 141,
  drop_type         = 370,
  drop_money        = 30,
  drop_item         = 73,
  union_number      = 73,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 9,
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