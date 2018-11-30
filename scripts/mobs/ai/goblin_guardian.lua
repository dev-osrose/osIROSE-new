registerNpc(219, {
  walk_speed        = 0,
  run_speed         = 310,
  scale             = 730,
  r_weapon          = 330,
  l_weapon          = 173,
  level             = 0,
  hp                = 105,
  attack            = 426,
  hit               = 599,
  def               = 410,
  res               = 563,
  avoid             = 289,
  attack_spd        = 185,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 260,
  drop_type         = 2217,
  drop_money        = 275,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 85,
  sell_tab0         = 85,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 15,
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