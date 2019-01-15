registerNpc(383, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 1100,
  r_weapon          = 280,
  l_weapon          = 0,
  level             = 0,
  hp                = 120,
  attack            = 217,
  hit               = 695,
  def               = 336,
  res               = 897,
  avoid             = 364,
  attack_spd        = 157,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 338,
  drop_type         = 971,
  drop_money        = 395,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 80,
  sell_tab0         = 80,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 11,
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