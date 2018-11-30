registerNpc(52, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 326,
  r_weapon          = 125,
  l_weapon          = 1074,
  level             = 0,
  hp                = 46,
  attack            = 30,
  hit               = 215,
  def               = 114,
  res               = 113,
  avoid             = 90,
  attack_spd        = 71,
  is_magic_damage   = 90,
  ai_type           = 1,
  give_exp          = 28,
  drop_type         = 40,
  drop_money        = 122,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 58,
  sell_tab0         = 58,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1250,
  hit_material_type = 3,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
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