registerNpc(287, {
  walk_speed        = 340,
  run_speed         = 850,
  scale             = 300,
  r_weapon          = 174,
  l_weapon          = 0,
  level             = 105,
  hp                = 207,
  attack            = 599,
  hit               = 316,
  def               = 563,
  res               = 309,
  avoid             = 134,
  attack_spd        = 115,
  is_magic_damage   = 0,
  ai_type           = 69,
  give_exp          = 912,
  drop_type         = 241,
  drop_money        = 15,
  drop_item         = 82,
  union_number      = 82,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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