registerNpc(257, {
  walk_speed        = 0,
  run_speed         = 480,
  scale             = 800,
  r_weapon          = 105,
  l_weapon          = 1076,
  level             = 0,
  hp                = 50,
  attack            = 27,
  hit               = 208,
  def               = 125,
  res               = 106,
  avoid             = 111,
  attack_spd        = 96,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 7,
  drop_type         = 36,
  drop_money        = 222,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 52,
  sell_tab0         = 52,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 5,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 14,
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