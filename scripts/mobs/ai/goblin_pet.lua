registerNpc(262, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 370,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 50,
  attack            = 34,
  hit               = 203,
  def               = 140,
  res               = 187,
  avoid             = 108,
  attack_spd        = 40,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 1,
  drop_type         = 29,
  drop_money        = 225,
  drop_item         = 75,
  union_number      = 75,
  need_summon_count = 50,
  sell_tab0         = 50,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 900,
  hit_material_type = 2,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 14,
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