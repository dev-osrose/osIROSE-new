registerNpc(13, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 266,
  r_weapon          = 60,
  l_weapon          = 0,
  level             = 0,
  hp                = 7,
  attack            = 17,
  hit               = 12,
  def               = 63,
  res               = 41,
  avoid             = 17,
  attack_spd        = 8,
  is_magic_damage   = 80,
  ai_type           = 0,
  give_exp          = 2,
  drop_type         = 11,
  drop_money        = 106,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
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