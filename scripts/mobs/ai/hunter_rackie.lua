registerNpc(96, {
  walk_speed        = 160,
  run_speed         = 400,
  scale             = 96,
  r_weapon          = 941,
  l_weapon          = 0,
  level             = 26,
  hp                = 20,
  attack            = 121,
  hit               = 102,
  def               = 70,
  res               = 34,
  avoid             = 50,
  attack_spd        = 85,
  is_magic_damage   = 0,
  ai_type           = 28,
  give_exp          = 28,
  drop_type         = 146,
  drop_money        = 30,
  drop_item         = 63,
  union_number      = 63,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2000,
  npc_type          = 6,
  hit_material_type = 0,
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