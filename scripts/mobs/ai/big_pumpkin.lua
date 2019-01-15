registerNpc(22, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 260,
  r_weapon          = 60,
  l_weapon          = 0,
  level             = 0,
  hp                = 10,
  attack            = 35,
  hit               = 47,
  def               = 74,
  res               = 31,
  avoid             = 27,
  attack_spd        = 10,
  is_magic_damage   = 85,
  ai_type           = 0,
  give_exp          = 2,
  drop_type         = 22,
  drop_money        = 109,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 2,
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