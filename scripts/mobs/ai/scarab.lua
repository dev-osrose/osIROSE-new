registerNpc(210, {
  walk_speed        = 300,
  run_speed         = 500,
  scale             = 50,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 35,
  hp                = 26,
  attack            = 173,
  hit               = 121,
  def               = 110,
  res               = 45,
  avoid             = 37,
  attack_spd        = 130,
  is_magic_damage   = 0,
  ai_type           = 58,
  give_exp          = 40,
  drop_type         = 205,
  drop_money        = 1,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 130,
  npc_type          = 4,
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