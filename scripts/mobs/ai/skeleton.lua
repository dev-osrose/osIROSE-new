registerNpc(305, {
  walk_speed        = 280,
  run_speed         = 470,
  scale             = 80,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 120,
  hp                = 80,
  attack            = 241,
  hit               = 148,
  def               = 195,
  res               = 154,
  avoid             = 70,
  attack_spd        = 130,
  is_magic_damage   = 0,
  ai_type           = 89,
  give_exp          = 1,
  drop_type         = 0,
  drop_money        = 1,
  drop_item         = 90,
  union_number      = 90,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 15,
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