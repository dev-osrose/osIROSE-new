registerNpc(207, {
  walk_speed        = 0,
  run_speed         = 290,
  scale             = 850,
  r_weapon          = 220,
  l_weapon          = 167,
  level             = 0,
  hp                = 40,
  attack            = 160,
  hit               = 219,
  def               = 150,
  res               = 155,
  avoid             = 106,
  attack_spd        = 49,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 324,
  drop_type         = 607,
  drop_money        = 202,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 80,
  sell_tab0         = 80,
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