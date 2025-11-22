registerNpc(4, {
  walk_speed        = 220,
  run_speed         = 490,
  scale             = 45,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 4,
  hp                = 10,
  attack            = 7,
  hit               = 78,
  def               = 29,
  res               = 5,
  avoid             = 10,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 104,
  give_exp          = 10,
  drop_type         = 0,
  drop_money        = 15,
  drop_item         = 73,
  union_number      = 73,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 200,
  npc_type          = 4,
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