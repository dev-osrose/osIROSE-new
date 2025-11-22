registerNpc(44, {
  walk_speed        = 0,
  run_speed         = 0,
  scale             = 120,
  r_weapon          = 1073,
  l_weapon          = 0,
  level             = 18,
  hp                = 34,
  attack            = 92,
  hit               = 89,
  def               = 66,
  res               = 26,
  avoid             = 39,
  attack_spd        = 95,
  is_magic_damage   = 1,
  ai_type           = 18,
  give_exp          = 31,
  drop_type         = 119,
  drop_money        = 1,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2500,
  npc_type          = 8,
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