registerNpc(183, {
  walk_speed        = 170,
  run_speed         = 700,
  scale             = 115,
  r_weapon          = 1015,
  l_weapon          = 0,
  level             = 52,
  hp                = 31,
  attack            = 243,
  hit               = 124,
  def               = 128,
  res               = 104,
  avoid             = 79,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 320,
  give_exp          = 53,
  drop_type         = 193,
  drop_money        = 28,
  drop_item         = 56,
  union_number      = 56,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 3,
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