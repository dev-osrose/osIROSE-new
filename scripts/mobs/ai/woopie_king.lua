registerNpc(205, {
  walk_speed        = 230,
  run_speed         = 700,
  scale             = 170,
  r_weapon          = 1003,
  l_weapon          = 302,
  level             = 25,
  hp                = 74,
  attack            = 137,
  hit               = 121,
  def               = 99,
  res               = 56,
  avoid             = 33,
  attack_spd        = 85,
  is_magic_damage   = 0,
  ai_type           = 55,
  give_exp          = 165,
  drop_type         = 200,
  drop_money        = 20,
  drop_item         = 70,
  union_number      = 70,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 10,
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