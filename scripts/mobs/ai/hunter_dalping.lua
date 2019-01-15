registerNpc(33, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 314,
  r_weapon          = 100,
  l_weapon          = 1071,
  level             = 0,
  hp                = 24,
  attack            = 20,
  hit               = 112,
  def               = 105,
  res               = 65,
  avoid             = 28,
  attack_spd        = 48,
  is_magic_damage   = 80,
  ai_type           = 0,
  give_exp          = 28,
  drop_type         = 29,
  drop_money        = 114,
  drop_item         = 9,
  union_number      = 9,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1800,
  hit_material_type = 6,
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