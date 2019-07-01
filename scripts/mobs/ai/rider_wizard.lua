registerNpc(352, {
  walk_speed        = 275,
  run_speed         = 800,
  scale             = 190,
  r_weapon          = 1092,
  l_weapon          = 0,
  level             = 108,
  hp                = 28,
  attack            = 460,
  hit               = 239,
  def               = 462,
  res               = 402,
  avoid             = 165,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 130,
  give_exp          = 98,
  drop_type         = 387,
  drop_money        = 0,
  drop_item         = 49,
  union_number      = 49,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
  npc_type          = 7,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
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