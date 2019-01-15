registerNpc(650, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 800,
  r_weapon          = 250,
  l_weapon          = 0,
  level             = 0,
  hp                = 147,
  attack            = 43,
  hit               = 746,
  def               = 306,
  res               = 451,
  avoid             = 382,
  attack_spd        = 233,
  is_magic_damage   = 107,
  ai_type           = 0,
  give_exp          = 347,
  drop_type         = 276,
  drop_money        = 519,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 7,
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