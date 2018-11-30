registerNpc(403, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 450,
  r_weapon          = 350,
  l_weapon          = 1075,
  level             = 0,
  hp                = 60,
  attack            = 206,
  hit               = 324,
  def               = 209,
  res               = 221,
  avoid             = 137,
  attack_spd        = 95,
  is_magic_damage   = 120,
  ai_type           = 1,
  give_exp          = 73,
  drop_type         = 618,
  drop_money        = 303,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 90,
  sell_tab0         = 90,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 19,
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