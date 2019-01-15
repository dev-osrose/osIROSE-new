registerNpc(34, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 338,
  r_weapon          = 115,
  l_weapon          = 1071,
  level             = 0,
  hp                = 26,
  attack            = 34,
  hit               = 129,
  def               = 102,
  res               = 86,
  avoid             = 36,
  attack_spd        = 49,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 28,
  drop_type         = 61,
  drop_money        = 115,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2000,
  hit_material_type = 8,
  face_icon         = 0,
  summon_mob_type   = 0,
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