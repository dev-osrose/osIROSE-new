registerNpc(216, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 320,
  r_weapon          = 138,
  l_weapon          = 0,
  level             = 70,
  hp                = 375,
  attack            = 387,
  hit               = 289,
  def               = 266,
  res               = 177,
  avoid             = 109,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 257,
  give_exp          = 1764,
  drop_type         = 272,
  drop_money        = 0,
  drop_item         = 82,
  union_number      = 82,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 10,
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