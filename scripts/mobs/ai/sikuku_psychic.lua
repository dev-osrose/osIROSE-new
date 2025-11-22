registerNpc(554, {
  walk_speed        = 220,
  run_speed         = 490,
  scale             = 100,
  r_weapon          = 1102,
  l_weapon          = 0,
  level             = 160,
  hp                = 31,
  attack            = 772,
  hit               = 474,
  def               = 422,
  res               = 783,
  avoid             = 258,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 295,
  give_exp          = 191,
  drop_type         = 473,
  drop_money        = 25,
  drop_item         = 64,
  union_number      = 64,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1600,
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