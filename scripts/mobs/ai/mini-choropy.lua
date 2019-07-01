registerNpc(11, {
  walk_speed        = 160,
  run_speed         = 242,
  scale             = 35,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 4,
  hp                = 15,
  attack            = 6,
  hit               = 59,
  def               = 29,
  res               = 13,
  avoid             = 6,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 1,
  give_exp          = 9,
  drop_type         = 104,
  drop_money        = 5,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 180,
  npc_type          = 1,
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