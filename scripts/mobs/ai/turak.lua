registerNpc(558, {
  walk_speed        = 300,
  run_speed         = 650,
  scale             = 190,
  r_weapon          = 142,
  l_weapon          = 0,
  level             = 175,
  hp                = 181,
  attack            = 953,
  hit               = 502,
  def               = 730,
  res               = 607,
  avoid             = 432,
  attack_spd        = 120,
  is_magic_damage   = 0,
  ai_type           = 298,
  give_exp          = 666,
  drop_type         = 477,
  drop_money        = 5,
  drop_item         = 75,
  union_number      = 75,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 270,
  npc_type          = 10,
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