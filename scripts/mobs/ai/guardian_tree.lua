registerNpc(208, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 800,
  r_weapon          = 340,
  l_weapon          = 1075,
  level             = 0,
  hp                = 57,
  attack            = 172,
  hit               = 313,
  def               = 185,
  res               = 216,
  avoid             = 153,
  attack_spd        = 69,
  is_magic_damage   = 90,
  ai_type           = 1,
  give_exp          = 57,
  drop_type         = 519,
  drop_money        = 203,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 75,
  sell_tab0         = 75,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
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