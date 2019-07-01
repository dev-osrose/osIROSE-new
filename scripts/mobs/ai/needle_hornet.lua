registerNpc(63, {
  walk_speed        = 240,
  run_speed         = 400,
  scale             = 70,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 15,
  hp                = 28,
  attack            = 75,
  hit               = 68,
  def               = 43,
  res               = 31,
  avoid             = 32,
  attack_spd        = 87,
  is_magic_damage   = 0,
  ai_type           = 5,
  give_exp          = 27,
  drop_type         = 126,
  drop_money        = 12,
  drop_item         = 64,
  union_number      = 64,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
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