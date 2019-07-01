registerNpc(181, {
  walk_speed        = 180,
  run_speed         = 700,
  scale             = 95,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 47,
  hp                = 28,
  attack            = 232,
  hit               = 142,
  def               = 146,
  res               = 64,
  avoid             = 48,
  attack_spd        = 115,
  is_magic_damage   = 0,
  ai_type           = 22,
  give_exp          = 50,
  drop_type         = 191,
  drop_money        = 20,
  drop_item         = 53,
  union_number      = 53,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 160,
  npc_type          = 4,
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