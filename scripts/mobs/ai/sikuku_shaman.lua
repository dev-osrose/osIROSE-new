registerNpc(553, {
  walk_speed        = 200,
  run_speed         = 450,
  scale             = 100,
  r_weapon          = 1101,
  l_weapon          = 0,
  level             = 150,
  hp                = 28,
  attack            = 714,
  hit               = 443,
  def               = 386,
  res               = 721,
  avoid             = 239,
  attack_spd        = 100,
  is_magic_damage   = 1,
  ai_type           = 295,
  give_exp          = 169,
  drop_type         = 472,
  drop_money        = 50,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1500,
  npc_type          = 7,
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