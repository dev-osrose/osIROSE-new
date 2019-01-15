registerNpc(128, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 510,
  r_weapon          = 145,
  l_weapon          = 1044,
  level             = 0,
  hp                = 70,
  attack            = 37,
  hit               = 323,
  def               = 194,
  res               = 192,
  avoid             = 102,
  attack_spd        = 114,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 264,
  drop_type         = 51,
  drop_money        = 175,
  drop_item         = 18,
  union_number      = 18,
  need_summon_count = 36,
  sell_tab0         = 36,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2000,
  hit_material_type = 6,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
  height            = 0
});
registerNpc(156, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 700,
  r_weapon          = 140,
  l_weapon          = 1044,
  level             = 0,
  hp                = 62,
  attack            = 24,
  hit               = 285,
  def               = 167,
  res               = 167,
  avoid             = 91,
  attack_spd        = 101,
  is_magic_damage   = 85,
  ai_type           = 0,
  give_exp          = 322,
  drop_type         = 46,
  drop_money        = 175,
  drop_item         = 18,
  union_number      = 18,
  need_summon_count = 52,
  sell_tab0         = 52,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2000,
  hit_material_type = 6,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
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