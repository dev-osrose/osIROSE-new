registerNpc(142, {
  walk_speed        = 240,
  run_speed         = 700,
  scale             = 155,
  r_weapon          = 104,
  l_weapon          = 0,
  level             = 60,
  hp                = 35,
  attack            = 245,
  hit               = 159,
  def               = 224,
  res               = 133,
  avoid             = 48,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 319,
  give_exp          = 50,
  drop_type         = 166,
  drop_money        = 16,
  drop_item         = 49,
  union_number      = 49,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 260,
  npc_type          = 2,
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