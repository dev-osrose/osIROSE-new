registerNpc(536, {
  walk_speed        = 0,
  run_speed         = 400,
  scale             = 850,
  r_weapon          = 120,
  l_weapon          = 1039,
  level             = 0,
  hp                = 140,
  attack            = 34,
  hit               = 677,
  def               = 412,
  res               = 447,
  avoid             = 351,
  attack_spd        = 220,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 292,
  drop_type         = 121,
  drop_money        = 454,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 35,
  sell_tab0         = 35,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 310,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
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