registerNpc(68, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 450,
  r_weapon          = 107,
  l_weapon          = 1061,
  level             = 0,
  hp                = 27,
  attack            = 34,
  hit               = 133,
  def               = 103,
  res               = 88,
  avoid             = 38,
  attack_spd        = 51,
  is_magic_damage   = 95,
  ai_type           = 1,
  give_exp          = 28,
  drop_type         = 56,
  drop_money        = 129,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 900,
  hit_material_type = 8,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
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