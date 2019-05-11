registerNpc(992, {
  walk_speed        = 0,
  run_speed         = 0,
  scale             = 130,
  r_weapon          = 1073,
  l_weapon          = 0,
  level             = 30,
  hp                = 2000,
  attack            = 10,
  hit               = 10,
  def               = 300,
  res               = 300,
  avoid             = 40,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 282,
  give_exp          = 0,
  drop_type         = 0,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 30,
  sell_tab0         = 30,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 10,
  npc_type          = 0,
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