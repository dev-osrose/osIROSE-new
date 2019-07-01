registerNpc(201, {
  walk_speed        = 300,
  run_speed         = 450,
  scale             = 430,
  r_weapon          = 1084,
  l_weapon          = 0,
  level             = 88,
  hp                = 195,
  attack            = 494,
  hit               = 256,
  def               = 460,
  res               = 249,
  avoid             = 110,
  attack_spd        = 85,
  is_magic_damage   = 1,
  ai_type           = 51,
  give_exp          = 666,
  drop_type         = 196,
  drop_money        = 1,
  drop_item         = 85,
  union_number      = 85,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 900,
  npc_type          = 10,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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