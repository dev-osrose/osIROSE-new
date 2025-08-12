registerNpc(5, {
  walk_speed        = 200,
  run_speed         = 450,
  scale             = 55,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 6,
  hp                = 11,
  attack            = 12,
  hit               = 56,
  def               = 33,
  res               = 17,
  avoid             = 15,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 104,
  give_exp          = 11,
  drop_type         = 103,
  drop_money        = 10,
  drop_item         = 75,
  union_number      = 75,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 3,
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