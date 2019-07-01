registerNpc(335, {
  walk_speed        = 250,
  run_speed         = 1050,
  scale             = 250,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 83,
  hp                = 81,
  attack            = 456,
  hit               = 229,
  def               = 362,
  res               = 194,
  avoid             = 103,
  attack_spd        = 200,
  is_magic_damage   = 0,
  ai_type           = 333,
  give_exp          = 84,
  drop_type         = 362,
  drop_money        = 0,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 10,
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