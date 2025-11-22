registerNpc(408, {
  walk_speed        = 300,
  run_speed         = 500,
  scale             = 460,
  r_weapon          = 1084,
  l_weapon          = 0,
  level             = 95,
  hp                = 282,
  attack            = 530,
  hit               = 334,
  def               = 475,
  res               = 235,
  avoid             = 163,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 76,
  give_exp          = 1320,
  drop_type         = 308,
  drop_money        = 1,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1200,
  npc_type          = 10,
  hit_material_type = 1,
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