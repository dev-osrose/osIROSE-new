registerNpc(157, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 700,
  r_weapon          = 155,
  l_weapon          = 1021,
  level             = 0,
  hp                = 70,
  attack            = 37,
  hit               = 345,
  def               = 183,
  res               = 218,
  avoid             = 110,
  attack_spd        = 111,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 112,
  drop_type         = 80,
  drop_money        = 176,
  drop_item         = 23,
  union_number      = 23,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 290,
  hit_material_type = 8,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
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