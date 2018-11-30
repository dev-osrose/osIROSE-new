registerNpc(272, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 850,
  r_weapon          = 120,
  l_weapon          = 1015,
  level             = 0,
  hp                = 55,
  attack            = 35,
  hit               = 224,
  def               = 149,
  res               = 205,
  avoid             = 120,
  attack_spd        = 44,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 328,
  drop_type         = 39,
  drop_money        = 227,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 55,
  sell_tab0         = 55,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 2,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 14,
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