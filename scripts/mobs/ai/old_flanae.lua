registerNpc(44, {
  walk_speed        = 0,
  run_speed         = 0,
  scale             = 0,
  r_weapon          = 120,
  l_weapon          = 1073,
  level             = 0,
  hp                = 18,
  attack            = 34,
  hit               = 92,
  def               = 89,
  res               = 66,
  avoid             = 26,
  attack_spd        = 39,
  is_magic_damage   = 95,
  ai_type           = 1,
  give_exp          = 18,
  drop_type         = 31,
  drop_money        = 119,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2500,
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