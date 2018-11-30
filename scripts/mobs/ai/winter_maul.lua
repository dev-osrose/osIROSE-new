registerNpc(368, {
  walk_speed        = 0,
  run_speed         = 245,
  scale             = 780,
  r_weapon          = 155,
  l_weapon          = 0,
  level             = 0,
  hp                = 105,
  attack            = 35,
  hit               = 540,
  def               = 263,
  res               = 619,
  avoid             = 179,
  attack_spd        = 115,
  is_magic_damage   = 130,
  ai_type           = 0,
  give_exp          = 143,
  drop_type         = 88,
  drop_money        = 383,
  drop_item         = 23,
  union_number      = 23,
  need_summon_count = 48,
  sell_tab0         = 48,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 3,
  face_icon         = 0,
  summon_mob_type   = 0,
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