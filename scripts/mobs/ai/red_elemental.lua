registerNpc(566, {
  walk_speed        = 200,
  run_speed         = 600,
  scale             = 200,
  r_weapon          = 1114,
  l_weapon          = 0,
  level             = 145,
  hp                = 37,
  attack            = 575,
  hit               = 458,
  def               = 410,
  res               = 691,
  avoid             = 267,
  attack_spd        = 80,
  is_magic_damage   = 0,
  ai_type           = 299,
  give_exp          = 121,
  drop_type         = 482,
  drop_money        = 40,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1200,
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