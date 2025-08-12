registerNpc(172, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 125,
  r_weapon          = 164,
  l_weapon          = 0,
  level             = 31,
  hp                = 26,
  attack            = 154,
  hit               = 114,
  def               = 99,
  res               = 39,
  avoid             = 33,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 320,
  give_exp          = 34,
  drop_type         = 187,
  drop_money        = 12,
  drop_item         = 63,
  union_number      = 63,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 360,
  npc_type          = 4,
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