registerNpc(274, {
  walk_speed        = 200,
  run_speed         = 750,
  scale             = 108,
  r_weapon          = 1080,
  l_weapon          = 0,
  level             = 52,
  hp                = 28,
  attack            = 232,
  hit               = 132,
  def               = 150,
  res               = 100,
  avoid             = 79,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 11,
  give_exp          = 39,
  drop_type         = 229,
  drop_money        = 15,
  drop_item         = 57,
  union_number      = 57,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 800,
  npc_type          = 1,
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