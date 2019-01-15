registerNpc(938, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 600,
  r_weapon          = 160,
  l_weapon          = 1074,
  level             = 0,
  hp                = 70,
  attack            = 1886,
  hit               = 295,
  def               = 215,
  res               = 191,
  avoid             = 90,
  attack_spd        = 87,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 169,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 35,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1400,
  hit_material_type = 0,
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