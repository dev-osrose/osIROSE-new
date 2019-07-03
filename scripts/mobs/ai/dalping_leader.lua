registerNpc(34, {
  walk_speed        = 240,
  run_speed         = 338,
  scale             = 115,
  r_weapon          = 1071,
  l_weapon          = 0,
  level             = 26,
  hp                = 34,
  attack            = 129,
  hit               = 102,
  def               = 86,
  res               = 36,
  avoid             = 49,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 28,
  give_exp          = 61,
  drop_type         = 115,
  drop_money        = 12,
  drop_item         = 62,
  union_number      = 62,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2000,
  npc_type          = 8,
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