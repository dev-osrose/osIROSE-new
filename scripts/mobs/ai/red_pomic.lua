registerNpc(73, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 700,
  r_weapon          = 90,
  l_weapon          = 1012,
  level             = 0,
  hp                = 15,
  attack            = 28,
  hit               = 75,
  def               = 68,
  res               = 43,
  avoid             = 31,
  attack_spd        = 32,
  is_magic_damage   = 80,
  ai_type           = 0,
  give_exp          = 3,
  drop_type         = 26,
  drop_money        = 133,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 68,
  sell_tab0         = 68,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 3,
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