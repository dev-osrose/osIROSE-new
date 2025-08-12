registerNpc(365, {
  walk_speed        = 240,
  run_speed         = 750,
  scale             = 155,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 83,
  hp                = 32,
  attack            = 379,
  hit               = 189,
  def               = 303,
  res               = 177,
  avoid             = 124,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 143,
  give_exp          = 71,
  drop_type         = 361,
  drop_money        = 0,
  drop_item         = 49,
  union_number      = 49,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 1,
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