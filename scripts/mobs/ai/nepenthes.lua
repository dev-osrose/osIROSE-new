registerNpc(521, {
  walk_speed        = 100,
  run_speed         = 120,
  scale             = 170,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 147,
  hp                = 37,
  attack            = 718,
  hit               = 460,
  def               = 446,
  res               = 417,
  avoid             = 271,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 301,
  give_exp          = 119,
  drop_type         = 445,
  drop_money        = 35,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
  npc_type          = 1,
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