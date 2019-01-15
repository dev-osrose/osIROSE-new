registerNpc(348, {
  walk_speed        = 0,
  run_speed         = 215,
  scale             = 700,
  r_weapon          = 150,
  l_weapon          = 1091,
  level             = 0,
  hp                = 100,
  attack            = 52,
  hit               = 533,
  def               = 252,
  res               = 582,
  avoid             = 227,
  attack_spd        = 127,
  is_magic_damage   = 110,
  ai_type           = 1,
  give_exp          = 129,
  drop_type         = 154,
  drop_money        = 380,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 53,
  sell_tab0         = 53,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1600,
  hit_material_type = 9,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 16,
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