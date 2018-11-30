registerNpc(351, {
  walk_speed        = 0,
  run_speed         = 280,
  scale             = 800,
  r_weapon          = 190,
  l_weapon          = 1036,
  level             = 0,
  hp                = 105,
  attack            = 35,
  hit               = 490,
  def               = 233,
  res               = 582,
  avoid             = 240,
  attack_spd        = 160,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 131,
  drop_type         = 112,
  drop_money        = 384,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 48,
  sell_tab0         = 48,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 16,
  height            = 0
});
registerNpc(936, {
  walk_speed        = 0,
  run_speed         = 280,
  scale             = 650,
  r_weapon          = 200,
  l_weapon          = 1036,
  level             = 0,
  hp                = 100,
  attack            = 4409,
  hit               = 401,
  def               = 260,
  res               = 367,
  avoid             = 170,
  attack_spd        = 135,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 168,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 45,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
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