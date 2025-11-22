registerNpc(202, {
  walk_speed        = 230,
  run_speed         = 400,
  scale             = 180,
  r_weapon          = 1084,
  l_weapon          = 0,
  level             = 78,
  hp                = 32,
  attack            = 391,
  hit               = 204,
  def               = 251,
  res               = 121,
  avoid             = 82,
  attack_spd        = 95,
  is_magic_damage   = 1,
  ai_type           = 52,
  give_exp          = 56,
  drop_type         = 197,
  drop_money        = 1,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1200,
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