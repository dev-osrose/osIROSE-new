registerNpc(52, {
  walk_speed        = 230,
  run_speed         = 326,
  scale             = 125,
  r_weapon          = 1074,
  l_weapon          = 0,
  level             = 46,
  hp                = 30,
  attack            = 215,
  hit               = 114,
  def               = 113,
  res               = 90,
  avoid             = 71,
  attack_spd        = 90,
  is_magic_damage   = 1,
  ai_type           = 28,
  give_exp          = 40,
  drop_type         = 122,
  drop_money        = 12,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1250,
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