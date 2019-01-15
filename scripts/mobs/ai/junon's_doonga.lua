registerNpc(407, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 700,
  r_weapon          = 400,
  l_weapon          = 134,
  level             = 6,
  hp                = 85,
  attack            = 273,
  hit               = 478,
  def               = 307,
  res               = 418,
  avoid             = 214,
  attack_spd        = 147,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 68,
  drop_type         = 1609,
  drop_money        = 307,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 85,
  sell_tab0         = 85,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 19,
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