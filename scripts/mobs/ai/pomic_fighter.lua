registerNpc(75, {
  walk_speed        = 160,
  run_speed         = 700,
  scale             = 102,
  r_weapon          = 1032,
  l_weapon          = 0,
  level             = 19,
  hp                = 27,
  attack            = 93,
  hit               = 74,
  def               = 51,
  res               = 37,
  avoid             = 39,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 5,
  give_exp          = 28,
  drop_type         = 135,
  drop_money        = 35,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 320,
  npc_type          = 3,
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