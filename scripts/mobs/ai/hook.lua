registerNpc(516, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 180,
  l_weapon          = 0,
  level             = 0,
  hp                = 145,
  attack            = 31,
  hit               = 713,
  def               = 416,
  res               = 394,
  avoid             = 499,
  attack_spd        = 243,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 293,
  drop_type         = 122,
  drop_money        = 441,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 40,
  sell_tab0         = 40,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 240,
  hit_material_type = 3,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 20,
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