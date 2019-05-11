registerNpc(403, {
  walk_speed        = 240,
  run_speed         = 450,
  scale             = 350,
  r_weapon          = 1075,
  l_weapon          = 0,
  level             = 60,
  hp                = 206,
  attack            = 324,
  hit               = 209,
  def               = 221,
  res               = 137,
  avoid             = 95,
  attack_spd        = 120,
  is_magic_damage   = 1,
  ai_type           = 73,
  give_exp          = 618,
  drop_type         = 303,
  drop_money        = 1,
  drop_item         = 90,
  union_number      = 90,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
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