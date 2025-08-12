registerNpc(203, {
  walk_speed        = 200,
  run_speed         = 360,
  scale             = 170,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 13,
  hp                = 49,
  attack            = 74,
  hit               = 84,
  def               = 57,
  res               = 34,
  avoid             = 21,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 54,
  give_exp          = 64,
  drop_type         = 198,
  drop_money        = 1,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
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