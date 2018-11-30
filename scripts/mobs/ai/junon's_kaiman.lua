registerNpc(404, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 680,
  r_weapon          = 350,
  l_weapon          = 105,
  level             = 0,
  hp                = 70,
  attack            = 214,
  hit               = 371,
  def               = 233,
  res               = 255,
  avoid             = 154,
  attack_spd        = 109,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 67,
  drop_type         = 658,
  drop_money        = 304,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 90,
  sell_tab0         = 90,
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