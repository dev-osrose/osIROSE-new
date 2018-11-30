registerNpc(102, {
  walk_speed        = 0,
  run_speed         = 190,
  scale             = 480,
  r_weapon          = 96,
  l_weapon          = 1080,
  level             = 0,
  hp                = 17,
  attack            = 23,
  hit               = 75,
  def               = 74,
  res               = 37,
  avoid             = 40,
  attack_spd        = 46,
  is_magic_damage   = 85,
  ai_type           = 0,
  give_exp          = 26,
  drop_type         = 28,
  drop_money        = 148,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 63,
  sell_tab0         = 63,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 5,
  face_icon         = 1,
  summon_mob_type   = 1,
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