registerNpc(632, {
  walk_speed        = 300,
  run_speed         = 770,
  scale             = 130,
  r_weapon          = 1010,
  l_weapon          = 305,
  level             = 123,
  hp                = 37,
  attack            = 584,
  hit               = 271,
  def               = 408,
  res               = 295,
  avoid             = 190,
  attack_spd        = 106,
  is_magic_damage   = 0,
  ai_type           = 339,
  give_exp          = 276,
  drop_type         = 501,
  drop_money        = 12,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 500,
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