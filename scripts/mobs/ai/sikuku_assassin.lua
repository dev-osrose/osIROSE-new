registerNpc(548, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 110,
  l_weapon          = 1111,
  level             = 0,
  hp                = 155,
  attack            = 35,
  hit               = 773,
  def               = 524,
  res               = 399,
  avoid             = 443,
  attack_spd        = 301,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 291,
  drop_type         = 148,
  drop_money        = 467,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 50,
  sell_tab0         = 50,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 5,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 21,
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