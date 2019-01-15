registerNpc(213, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 700,
  r_weapon          = 300,
  l_weapon          = 167,
  level             = 0,
  hp                = 42,
  attack            = 161,
  hit               = 230,
  def               = 154,
  res               = 162,
  avoid             = 112,
  attack_spd        = 51,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 318,
  drop_type         = 619,
  drop_money        = 202,
  drop_item         = 10,
  union_number      = 10,
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
  quest_type        = 15,
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