registerNpc(913, {
  walk_speed        = 0,
  run_speed         = 145,
  scale             = 330,
  r_weapon          = 100,
  l_weapon          = 1005,
  level             = 0,
  hp                = 44,
  attack            = 1473,
  hit               = 160,
  def               = 116,
  res               = 119,
  avoid             = 56,
  attack_spd        = 57,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 167,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 30,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 0,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 0,
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