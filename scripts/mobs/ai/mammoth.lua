registerNpc(376, {
  walk_speed        = 0,
  run_speed         = 310,
  scale             = 1050,
  r_weapon          = 200,
  l_weapon          = 0,
  level             = 0,
  hp                = 84,
  attack            = 35,
  hit               = 401,
  def               = 179,
  res               = 267,
  avoid             = 184,
  attack_spd        = 126,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 136,
  drop_type         = 77,
  drop_money        = 363,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 51,
  sell_tab0         = 51,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 3,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 17,
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