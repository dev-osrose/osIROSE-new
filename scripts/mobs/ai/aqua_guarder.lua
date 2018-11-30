registerNpc(925, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 410,
  r_weapon          = 115,
  l_weapon          = 1035,
  level             = 0,
  hp                = 48,
  attack            = 1894,
  hit               = 151,
  def               = 118,
  res               = 167,
  avoid             = 73,
  attack_spd        = 55,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 167,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 31,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
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