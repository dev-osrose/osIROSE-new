registerNpc(294, {
  walk_speed        = 230,
  run_speed         = 500,
  scale             = 90,
  r_weapon          = 1096,
  l_weapon          = 0,
  level             = 50,
  hp                = 350,
  attack            = 1,
  hit               = 10,
  def               = 100,
  res               = 50,
  avoid             = 60,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 165,
  give_exp          = 1,
  drop_type         = 0,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1600,
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