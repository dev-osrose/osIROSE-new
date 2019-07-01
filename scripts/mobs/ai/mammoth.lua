registerNpc(376, {
  walk_speed        = 310,
  run_speed         = 1050,
  scale             = 200,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 84,
  hp                = 35,
  attack            = 401,
  hit               = 179,
  def               = 267,
  res               = 184,
  avoid             = 126,
  attack_spd        = 115,
  is_magic_damage   = 0,
  ai_type           = 136,
  give_exp          = 77,
  drop_type         = 363,
  drop_money        = 15,
  drop_item         = 51,
  union_number      = 51,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 3,
  hit_material_type = 2,
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