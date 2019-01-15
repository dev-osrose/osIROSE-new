registerNpc(82, {
  walk_speed        = 0,
  run_speed         = 145,
  scale             = 700,
  r_weapon          = 80,
  l_weapon          = 0,
  level             = 0,
  hp                = 22,
  attack            = 27,
  hit               = 106,
  def               = 78,
  res               = 57,
  avoid             = 43,
  attack_spd        = 40,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 5,
  drop_type         = 30,
  drop_money        = 137,
  drop_item         = 19,
  union_number      = 19,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 3,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 11,
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