registerNpc(529, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 300,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 158,
  hp                = 42,
  attack            = 821,
  hit               = 455,
  def               = 497,
  res               = 402,
  avoid             = 270,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 296,
  give_exp          = 140,
  drop_type         = 451,
  drop_money        = 30,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 3,
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