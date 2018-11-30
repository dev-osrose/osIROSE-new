registerNpc(92, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 520,
  r_weapon          = 98,
  l_weapon          = 1014,
  level             = 0,
  hp                = 26,
  attack            = 25,
  hit               = 130,
  def               = 105,
  res               = 86,
  avoid             = 32,
  attack_spd        = 28,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 24,
  drop_type         = 31,
  drop_money        = 143,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 61,
  sell_tab0         = 61,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 4,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
  height            = 0
});
registerNpc(94, {
  walk_speed        = 0,
  run_speed         = 140,
  scale             = 330,
  r_weapon          = 100,
  l_weapon          = 1014,
  level             = 0,
  hp                = 25,
  attack            = 25,
  hit               = 126,
  def               = 114,
  res               = 83,
  avoid             = 24,
  attack_spd        = 27,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 24,
  drop_type         = 29,
  drop_money        = 145,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 4,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 18,
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