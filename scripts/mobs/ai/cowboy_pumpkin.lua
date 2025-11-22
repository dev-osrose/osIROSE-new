registerNpc(24, {
  walk_speed        = 200,
  run_speed         = 260,
  scale             = 120,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 14,
  hp                = 25,
  attack            = 75,
  hit               = 85,
  def               = 56,
  res               = 18,
  avoid             = 18,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 23,
  give_exp          = 27,
  drop_type         = 111,
  drop_money        = 15,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
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