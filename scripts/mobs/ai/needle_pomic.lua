registerNpc(71, {
  walk_speed        = 140,
  run_speed         = 400,
  scale             = 80,
  r_weapon          = 1011,
  l_weapon          = 0,
  level             = 12,
  hp                = 25,
  attack            = 60,
  hit               = 70,
  def               = 45,
  res               = 24,
  avoid             = 29,
  attack_spd        = 85,
  is_magic_damage   = 0,
  ai_type           = 2,
  give_exp          = 25,
  drop_type         = 131,
  drop_money        = 20,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 1,
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