registerNpc(526, {
  walk_speed        = 240,
  run_speed         = 530,
  scale             = 170,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 156,
  hp                = 35,
  attack            = 752,
  hit               = 441,
  def               = 473,
  res               = 329,
  avoid             = 239,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 300,
  give_exp          = 126,
  drop_type         = 448,
  drop_money        = 25,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 220,
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