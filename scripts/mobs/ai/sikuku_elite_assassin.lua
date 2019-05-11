registerNpc(549, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 100,
  r_weapon          = 1111,
  l_weapon          = 0,
  level             = 180,
  hp                = 36,
  attack            = 891,
  hit               = 616,
  def               = 489,
  res               = 542,
  avoid             = 358,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 291,
  give_exp          = 144,
  drop_type         = 468,
  drop_money        = 30,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 2000,
  npc_type          = 6,
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