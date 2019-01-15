registerNpc(201, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 450,
  r_weapon          = 430,
  l_weapon          = 1084,
  level             = 0,
  hp                = 88,
  attack            = 195,
  hit               = 494,
  def               = 256,
  res               = 460,
  avoid             = 249,
  attack_spd        = 110,
  is_magic_damage   = 85,
  ai_type           = 1,
  give_exp          = 51,
  drop_type         = 666,
  drop_money        = 196,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 85,
  sell_tab0         = 85,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 900,
  hit_material_type = 10,
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