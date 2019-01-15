registerNpc(544, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 120,
  l_weapon          = 1109,
  level             = 0,
  hp                = 173,
  attack            = 43,
  hit               = 815,
  def               = 516,
  res               = 559,
  avoid             = 475,
  attack_spd        = 283,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 297,
  drop_type         = 162,
  drop_money        = 463,
  drop_item         = 27,
  union_number      = 27,
  need_summon_count = 44,
  sell_tab0         = 44,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 270,
  hit_material_type = 3,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 21,
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