registerNpc(103, {
  walk_speed        = 170,
  run_speed         = 700,
  scale             = 110,
  r_weapon          = 1003,
  l_weapon          = 302,
  level             = 18,
  hp                = 34,
  attack            = 76,
  hit               = 89,
  def               = 66,
  res               = 26,
  avoid             = 39,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 23,
  give_exp          = 40,
  drop_type         = 149,
  drop_money        = 35,
  drop_item         = 65,
  union_number      = 65,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
  npc_type          = 8,
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