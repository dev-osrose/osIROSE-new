registerNpc(154, {
  walk_speed        = 220,
  run_speed         = 700,
  scale             = 130,
  r_weapon          = 1002,
  l_weapon          = 0,
  level             = 62,
  hp                = 32,
  attack            = 291,
  hit               = 140,
  def               = 156,
  res               = 127,
  avoid             = 93,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 5,
  give_exp          = 53,
  drop_type         = 173,
  drop_money        = 15,
  drop_item         = 52,
  union_number      = 52,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 240,
  npc_type          = 3,
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