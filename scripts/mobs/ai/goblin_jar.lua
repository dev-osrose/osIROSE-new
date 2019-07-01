registerNpc(261, {
  walk_speed        = 150,
  run_speed         = 350,
  scale             = 100,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 45,
  hp                = 33,
  attack            = 183,
  hit               = 131,
  def               = 169,
  res               = 96,
  avoid             = 36,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 1,
  give_exp          = 27,
  drop_type         = 224,
  drop_money        = 60,
  drop_item         = 52,
  union_number      = 52,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 800,
  npc_type          = 2,
  hit_material_type = 2,
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