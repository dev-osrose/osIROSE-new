registerNpc(131, {
  walk_speed        = 180,
  run_speed         = 600,
  scale             = 80,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 19,
  hp                = 25,
  attack            = 88,
  hit               = 80,
  def               = 61,
  res               = 35,
  avoid             = 37,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 4,
  give_exp          = 39,
  drop_type         = 157,
  drop_money        = 20,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 180,
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