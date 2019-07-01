registerNpc(51, {
  walk_speed        = 220,
  run_speed         = 314,
  scale             = 100,
  r_weapon          = 1074,
  l_weapon          = 0,
  level             = 44,
  hp                = 27,
  attack            = 196,
  hit               = 119,
  def               = 126,
  res               = 83,
  avoid             = 68,
  attack_spd        = 95,
  is_magic_damage   = 1,
  ai_type           = 26,
  give_exp          = 40,
  drop_type         = 121,
  drop_money        = 10,
  drop_item         = 56,
  union_number      = 56,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1200,
  npc_type          = 1,
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