registerNpc(555, {
  walk_speed        = 210,
  run_speed         = 470,
  scale             = 110,
  r_weapon          = 1103,
  l_weapon          = 0,
  level             = 183,
  hp                = 37,
  attack            = 909,
  hit               = 549,
  def               = 507,
  res               = 930,
  avoid             = 303,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 295,
  give_exp          = 229,
  drop_type         = 474,
  drop_money        = 30,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1700,
  npc_type          = 7,
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