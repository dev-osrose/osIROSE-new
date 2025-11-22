registerNpc(133, {
  walk_speed        = 185,
  run_speed         = 700,
  scale             = 120,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 35,
  hp                = 26,
  attack            = 173,
  hit               = 121,
  def               = 110,
  res               = 45,
  avoid             = 37,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 5,
  give_exp          = 42,
  drop_type         = 159,
  drop_money        = 25,
  drop_item         = 63,
  union_number      = 63,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 210,
  npc_type          = 4,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
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