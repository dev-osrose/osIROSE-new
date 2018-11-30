registerNpc(572, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 190,
  l_weapon          = 1075,
  level             = 0,
  hp                = 157,
  attack            = 35,
  hit               = 754,
  def               = 481,
  res               = 502,
  avoid             = 509,
  attack_spd        = 293,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 303,
  drop_type         = 139,
  drop_money        = 486,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 25,
  sell_tab0         = 25,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 20,
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