registerNpc(256, {
  walk_speed        = 0,
  run_speed         = 450,
  scale             = 750,
  r_weapon          = 90,
  l_weapon          = 1076,
  level             = 0,
  hp                = 46,
  attack            = 27,
  hit               = 205,
  def               = 122,
  res               = 132,
  avoid             = 87,
  attack_spd        = 71,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 7,
  drop_type         = 35,
  drop_money        = 221,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 55,
  sell_tab0         = 55,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 180,
  hit_material_type = 1,
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