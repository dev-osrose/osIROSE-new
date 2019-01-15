registerNpc(131, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 600,
  r_weapon          = 80,
  l_weapon          = 0,
  level             = 0,
  hp                = 19,
  attack            = 25,
  hit               = 88,
  def               = 80,
  res               = 61,
  avoid             = 35,
  attack_spd        = 37,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 4,
  drop_type         = 39,
  drop_money        = 157,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 180,
  hit_material_type = 1,
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