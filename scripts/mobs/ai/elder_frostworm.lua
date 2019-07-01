registerNpc(362, {
  walk_speed        = 250,
  run_speed         = 1050,
  scale             = 300,
  r_weapon          = 1094,
  l_weapon          = 0,
  level             = 118,
  hp                = 39,
  attack            = 582,
  hit               = 245,
  def               = 618,
  res               = 285,
  avoid             = 181,
  attack_spd        = 95,
  is_magic_damage   = 1,
  ai_type           = 135,
  give_exp          = 132,
  drop_type         = 394,
  drop_money        = 0,
  drop_item         = 55,
  union_number      = 55,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
  npc_type          = 3,
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