registerNpc(555, {
  walk_speed        = 0,
  run_speed         = 210,
  scale             = 470,
  r_weapon          = 110,
  l_weapon          = 1103,
  level             = 0,
  hp                = 183,
  attack            = 37,
  hit               = 909,
  def               = 549,
  res               = 507,
  avoid             = 930,
  attack_spd        = 303,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 295,
  drop_type         = 229,
  drop_money        = 474,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 70,
  sell_tab0         = 70,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1700,
  hit_material_type = 7,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 22,
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