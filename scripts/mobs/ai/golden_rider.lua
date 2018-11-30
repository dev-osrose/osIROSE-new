registerNpc(394, {
  walk_speed        = 0,
  run_speed         = 290,
  scale             = 670,
  r_weapon          = 225,
  l_weapon          = 1036,
  level             = 0,
  hp                = 112,
  attack            = 139,
  hit               = 643,
  def               = 370,
  res               = 802,
  avoid             = 246,
  attack_spd        = 175,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 132,
  drop_type         = 509,
  drop_money        = 385,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 99,
  sell_tab0         = 99,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
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