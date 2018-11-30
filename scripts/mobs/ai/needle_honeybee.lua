registerNpc(61, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 350,
  r_weapon          = 62,
  l_weapon          = 0,
  level             = 0,
  hp                = 11,
  attack            = 25,
  hit               = 56,
  def               = 68,
  res               = 43,
  avoid             = 23,
  attack_spd        = 28,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 2,
  drop_type         = 25,
  drop_money        = 124,
  drop_item         = 8,
  union_number      = 8,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 1,
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