registerNpc(350, {
  walk_speed        = 215,
  run_speed         = 700,
  scale             = 200,
  r_weapon          = 1091,
  l_weapon          = 0,
  level             = 108,
  hp                = 86,
  attack            = 607,
  hit               = 288,
  def               = 717,
  res               = 268,
  avoid             = 138,
  attack_spd        = 135,
  is_magic_damage   = 0,
  ai_type           = 334,
  give_exp          = 103,
  drop_type         = 386,
  drop_money        = 0,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 10,
  hit_material_type = 1,
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