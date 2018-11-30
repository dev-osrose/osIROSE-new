registerNpc(756, {
  walk_speed        = 0,
  run_speed         = 140,
  scale             = 420,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 14,
  attack            = 38,
  hit               = 68,
  def               = 72,
  res               = 49,
  avoid             = 27,
  attack_spd        = 31,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 312,
  drop_type         = 31,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
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