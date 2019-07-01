registerNpc(361, {
  walk_speed        = 250,
  run_speed         = 750,
  scale             = 200,
  r_weapon          = 1094,
  l_weapon          = 0,
  level             = 115,
  hp                = 42,
  attack            = 493,
  hit               = 273,
  def               = 830,
  res               = 296,
  avoid             = 103,
  attack_spd        = 90,
  is_magic_damage   = 1,
  ai_type           = 335,
  give_exp          = 108,
  drop_type         = 393,
  drop_money        = 0,
  drop_item         = 53,
  union_number      = 53,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
  npc_type          = 2,
  hit_material_type = 2,
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