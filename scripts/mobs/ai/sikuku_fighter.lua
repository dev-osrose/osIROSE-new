registerNpc(538, {
  walk_speed        = 0,
  run_speed         = 400,
  scale             = 850,
  r_weapon          = 140,
  l_weapon          = 1105,
  level             = 0,
  hp                = 147,
  attack            = 40,
  hit               = 718,
  def               = 433,
  res               = 476,
  avoid             = 382,
  attack_spd        = 233,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 292,
  drop_type         = 148,
  drop_money        = 456,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 35,
  sell_tab0         = 35,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 310,
  hit_material_type = 3,
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