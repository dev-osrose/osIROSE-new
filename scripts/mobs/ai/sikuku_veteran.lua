registerNpc(539, {
  walk_speed        = 0,
  run_speed         = 420,
  scale             = 890,
  r_weapon          = 110,
  l_weapon          = 1106,
  level             = 0,
  hp                = 173,
  attack            = 43,
  hit               = 787,
  def               = 516,
  res               = 587,
  avoid             = 475,
  attack_spd        = 295,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 292,
  drop_type         = 134,
  drop_money        = 457,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 38,
  sell_tab0         = 38,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 330,
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