registerNpc(145, {
  walk_speed        = 210,
  run_speed         = 700,
  scale             = 170,
  r_weapon          = 1045,
  l_weapon          = 0,
  level             = 63,
  hp                = 24,
  attack            = 290,
  hit               = 169,
  def               = 170,
  res               = 93,
  avoid             = 103,
  attack_spd        = 85,
  is_magic_damage   = 0,
  ai_type           = 322,
  give_exp          = 46,
  drop_type         = 169,
  drop_money        = 20,
  drop_item         = 55,
  union_number      = 55,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2000,
  npc_type          = 6,
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