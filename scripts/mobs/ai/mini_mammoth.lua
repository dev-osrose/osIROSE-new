registerNpc(375, {
  walk_speed        = 260,
  run_speed         = 850,
  scale             = 150,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 78,
  hp                = 32,
  attack            = 355,
  hit               = 180,
  def               = 235,
  res               = 164,
  avoid             = 117,
  attack_spd        = 105,
  is_magic_damage   = 0,
  ai_type           = 139,
  give_exp          = 64,
  drop_type         = 358,
  drop_money        = 0,
  drop_item         = 52,
  union_number      = 52,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 230,
  npc_type          = 1,
  hit_material_type = 2,
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