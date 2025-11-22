registerNpc(143, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 170,
  r_weapon          = 105,
  l_weapon          = 0,
  level             = 64,
  hp                = 30,
  attack            = 318,
  hit               = 175,
  def               = 201,
  res               = 94,
  avoid             = 66,
  attack_spd        = 115,
  is_magic_damage   = 0,
  ai_type           = 320,
  give_exp          = 50,
  drop_type         = 167,
  drop_money        = 20,
  drop_item         = 52,
  union_number      = 52,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 280,
  npc_type          = 4,
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