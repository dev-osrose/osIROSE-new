registerNpc(32, {
  walk_speed        = 0,
  run_speed         = 210,
  scale             = 302,
  r_weapon          = 100,
  l_weapon          = 1071,
  level             = 0,
  hp                = 22,
  attack            = 23,
  hit               = 94,
  def               = 81,
  res               = 46,
  avoid             = 50,
  attack_spd        = 53,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 26,
  drop_type         = 29,
  drop_money        = 113,
  drop_item         = 7,
  union_number      = 7,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1800,
  hit_material_type = 5,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 11,
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