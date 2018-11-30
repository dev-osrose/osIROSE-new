registerNpc(45, {
  walk_speed        = 0,
  run_speed         = 0,
  scale             = 0,
  r_weapon          = 200,
  l_weapon          = 1073,
  level             = 0,
  hp                = 21,
  attack            = 31,
  hit               = 88,
  def               = 91,
  res               = 92,
  avoid             = 46,
  attack_spd        = 17,
  is_magic_damage   = 75,
  ai_type           = 1,
  give_exp          = 18,
  drop_type         = 19,
  drop_money        = 120,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2600,
  hit_material_type = 2,
  face_icon         = 1,
  summon_mob_type   = 1,
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