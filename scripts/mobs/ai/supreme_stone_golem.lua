registerNpc(762, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 700,
  r_weapon          = 182,
  l_weapon          = 0,
  level             = 0,
  hp                = 102,
  attack            = 35,
  hit               = 403,
  def               = 227,
  res               = 323,
  avoid             = 231,
  attack_spd        = 155,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 40,
  drop_type         = 31,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 190,
  hit_material_type = 1,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 18,
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