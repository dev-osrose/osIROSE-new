registerNpc(405, {
  walk_speed        = 260,
  run_speed         = 720,
  scale             = 340,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 80,
  hp                = 223,
  attack            = 419,
  hit               = 257,
  def               = 291,
  res               = 171,
  avoid             = 123,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 74,
  give_exp          = 618,
  drop_type         = 305,
  drop_money        = 1,
  drop_item         = 93,
  union_number      = 93,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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