registerNpc(282, {
  walk_speed        = 220,
  run_speed         = 750,
  scale             = 150,
  r_weapon          = 308,
  l_weapon          = 0,
  level             = 92,
  hp                = 27,
  attack            = 386,
  hit               = 207,
  def               = 289,
  res               = 334,
  avoid             = 138,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 119,
  give_exp          = 55,
  drop_type         = 237,
  drop_money        = 12,
  drop_item         = 45,
  union_number      = 45,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 7,
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