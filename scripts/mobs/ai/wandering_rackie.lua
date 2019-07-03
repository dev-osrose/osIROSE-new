registerNpc(204, {
  walk_speed        = 160,
  run_speed         = 540,
  scale             = 105,
  r_weapon          = 1011,
  l_weapon          = 0,
  level             = 20,
  hp                = 34,
  attack            = 101,
  hit               = 94,
  def               = 71,
  res               = 28,
  avoid             = 42,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 8,
  give_exp          = 51,
  drop_type         = 199,
  drop_money        = 25,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 8,
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