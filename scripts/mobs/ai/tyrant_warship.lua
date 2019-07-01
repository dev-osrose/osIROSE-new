registerNpc(356, {
  walk_speed        = 300,
  run_speed         = 950,
  scale             = 250,
  r_weapon          = 1093,
  l_weapon          = 0,
  level             = 114,
  hp                = 88,
  attack            = 644,
  hit               = 303,
  def               = 783,
  res               = 286,
  avoid             = 147,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 134,
  give_exp          = 384,
  drop_type         = 392,
  drop_money        = 0,
  drop_item         = 73,
  union_number      = 73,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1700,
  npc_type          = 10,
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