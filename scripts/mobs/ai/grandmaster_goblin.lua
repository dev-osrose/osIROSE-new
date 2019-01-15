registerNpc(287, {
  walk_speed        = 0,
  run_speed         = 340,
  scale             = 850,
  r_weapon          = 300,
  l_weapon          = 174,
  level             = 0,
  hp                = 105,
  attack            = 207,
  hit               = 599,
  def               = 316,
  res               = 563,
  avoid             = 309,
  attack_spd        = 134,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 69,
  drop_type         = 912,
  drop_money        = 241,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 82,
  sell_tab0         = 82,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
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