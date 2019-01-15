registerNpc(279, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 850,
  r_weapon          = 160,
  l_weapon          = 170,
  level             = 0,
  hp                = 89,
  attack            = 33,
  hit               = 451,
  def               = 227,
  res               = 366,
  avoid             = 144,
  attack_spd        = 95,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 117,
  drop_type         = 55,
  drop_money        = 234,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 43,
  sell_tab0         = 43,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 4,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 14,
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