registerNpc(144, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 152,
  r_weapon          = 1045,
  l_weapon          = 0,
  level             = 61,
  hp                = 28,
  attack            = 255,
  hit               = 144,
  def               = 133,
  res               = 139,
  avoid             = 115,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 321,
  give_exp          = 54,
  drop_type         = 168,
  drop_money        = 16,
  drop_item         = 53,
  union_number      = 53,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2400,
  npc_type          = 5,
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