registerNpc(283, {
  walk_speed        = 225,
  run_speed         = 750,
  scale             = 150,
  r_weapon          = 337,
  l_weapon          = 0,
  level             = 76,
  hp                = 48,
  attack            = 396,
  hit               = 200,
  def               = 249,
  res               = 164,
  avoid             = 93,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 119,
  give_exp          = 75,
  drop_type         = 238,
  drop_money        = 10,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1000,
  npc_type          = 9,
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