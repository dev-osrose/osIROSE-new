registerNpc(511, {
  walk_speed        = 250,
  run_speed         = 550,
  scale             = 200,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 143,
  hp                = 33,
  attack            = 604,
  hit               = 411,
  def               = 524,
  res               = 393,
  avoid             = 135,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 294,
  give_exp          = 118,
  drop_type         = 437,
  drop_money        = 20,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 240,
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