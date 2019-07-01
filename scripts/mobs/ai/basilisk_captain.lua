registerNpc(371, {
  walk_speed        = 190,
  run_speed         = 700,
  scale             = 200,
  r_weapon          = 1084,
  l_weapon          = 0,
  level             = 102,
  hp                = 52,
  attack            = 544,
  hit               = 256,
  def               = 601,
  res               = 232,
  avoid             = 130,
  attack_spd        = 95,
  is_magic_damage   = 0,
  ai_type           = 129,
  give_exp          = 127,
  drop_type         = 381,
  drop_money        = 31,
  drop_item         = 56,
  union_number      = 56,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 800,
  npc_type          = 9,
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