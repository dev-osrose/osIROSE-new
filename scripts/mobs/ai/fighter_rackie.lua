registerNpc(93, {
  walk_speed        = 145,
  run_speed         = 600,
  scale             = 100,
  r_weapon          = 1005,
  l_weapon          = 301,
  level             = 28,
  hp                = 28,
  attack            = 132,
  hit               = 87,
  def               = 70,
  res               = 53,
  avoid             = 48,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 23,
  give_exp          = 33,
  drop_type         = 144,
  drop_money        = 40,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
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