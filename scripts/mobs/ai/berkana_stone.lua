registerNpc(651, {
  walk_speed        = 100,
  run_speed         = 600,
  scale             = 130,
  r_weapon          = 1093,
  l_weapon          = 0,
  level             = 149,
  hp                = 32,
  attack            = 661,
  hit               = 330,
  def               = 404,
  res               = 596,
  avoid             = 237,
  attack_spd        = 108,
  is_magic_damage   = 0,
  ai_type           = 348,
  give_exp          = 276,
  drop_type         = 520,
  drop_money        = 12,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 900,
  npc_type          = 7,
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