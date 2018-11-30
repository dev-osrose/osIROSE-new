registerNpc(378, {
  walk_speed        = 0,
  run_speed         = 310,
  scale             = 1050,
  r_weapon          = 300,
  l_weapon          = 0,
  level             = 0,
  hp                = 86,
  attack            = 33,
  hit               = 435,
  def               = 221,
  res               = 338,
  avoid             = 137,
  attack_spd        = 92,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 136,
  drop_type         = 71,
  drop_money        = 366,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 54,
  sell_tab0         = 54,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 4,
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