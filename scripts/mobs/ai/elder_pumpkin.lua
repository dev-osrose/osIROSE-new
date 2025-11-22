registerNpc(23, {
  walk_speed        = 230,
  run_speed         = 400,
  scale             = 110,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 13,
  hp                = 28,
  attack            = 67,
  hit               = 65,
  def               = 39,
  res               = 28,
  avoid             = 30,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 21,
  give_exp          = 27,
  drop_type         = 110,
  drop_money        = 12,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
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