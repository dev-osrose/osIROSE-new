registerNpc(630, {
  walk_speed        = 500,
  run_speed         = 1050,
  scale             = 450,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 90,
  hp                = 197,
  attack            = 506,
  hit               = 260,
  def               = 472,
  res               = 256,
  avoid             = 112,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 276,
  give_exp          = 900,
  drop_type         = 334,
  drop_money        = 12,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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