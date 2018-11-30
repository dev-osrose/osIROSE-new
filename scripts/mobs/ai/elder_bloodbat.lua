registerNpc(258, {
  walk_speed        = 0,
  run_speed         = 500,
  scale             = 850,
  r_weapon          = 115,
  l_weapon          = 1076,
  level             = 0,
  hp                = 60,
  attack            = 32,
  hit               = 281,
  def               = 137,
  res               = 150,
  avoid             = 122,
  attack_spd        = 90,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 7,
  drop_type         = 43,
  drop_money        = 223,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 48,
  sell_tab0         = 48,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 3,
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