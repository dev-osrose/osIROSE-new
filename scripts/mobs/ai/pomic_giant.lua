registerNpc(76, {
  walk_speed        = 180,
  run_speed         = 700,
  scale             = 185,
  r_weapon          = 1012,
  l_weapon          = 0,
  level             = 24,
  hp                = 75,
  attack            = 132,
  hit               = 109,
  def               = 96,
  res               = 55,
  avoid             = 32,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 318,
  give_exp          = 182,
  drop_type         = 135,
  drop_money        = 40,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 3,
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