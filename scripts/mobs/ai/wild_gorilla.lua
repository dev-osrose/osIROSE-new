registerNpc(206, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 550,
  r_weapon          = 180,
  l_weapon          = 0,
  level             = 0,
  hp                = 43,
  attack            = 35,
  hit               = 209,
  def               = 131,
  res               = 132,
  avoid             = 62,
  attack_spd        = 72,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 8,
  drop_type         = 59,
  drop_money        = 201,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 8,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 15,
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