registerNpc(633, {
  walk_speed        = 300,
  run_speed         = 700,
  scale             = 130,
  r_weapon          = 1046,
  l_weapon          = 0,
  level             = 126,
  hp                = 40,
  attack            = 627,
  hit               = 262,
  def               = 369,
  res               = 311,
  avoid             = 195,
  attack_spd        = 109,
  is_magic_damage   = 0,
  ai_type           = 340,
  give_exp          = 276,
  drop_type         = 502,
  drop_money        = 12,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 500,
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