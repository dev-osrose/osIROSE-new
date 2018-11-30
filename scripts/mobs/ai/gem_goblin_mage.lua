registerNpc(282, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 750,
  r_weapon          = 150,
  l_weapon          = 308,
  level             = 0,
  hp                = 92,
  attack            = 27,
  hit               = 386,
  def               = 207,
  res               = 289,
  avoid             = 334,
  attack_spd        = 138,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 119,
  drop_type         = 55,
  drop_money        = 237,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 45,
  sell_tab0         = 45,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 7,
  face_icon         = 1,
  summon_mob_type   = 1,
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