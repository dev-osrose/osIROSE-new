registerNpc(396, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 710,
  r_weapon          = 430,
  l_weapon          = 1055,
  level             = 0,
  hp                = 155,
  attack            = 609,
  hit               = 929,
  def               = 569,
  res               = 1319,
  avoid             = 571,
  attack_spd        = 235,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 270,
  drop_type         = 3410,
  drop_money        = 278,
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
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 18,
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