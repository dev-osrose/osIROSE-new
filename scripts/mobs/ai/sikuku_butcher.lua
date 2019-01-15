registerNpc(545, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 120,
  l_weapon          = 1110,
  level             = 0,
  hp                = 174,
  attack            = 44,
  hit               = 820,
  def               = 519,
  res               = 564,
  avoid             = 479,
  attack_spd        = 285,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 297,
  drop_type         = 171,
  drop_money        = 464,
  drop_item         = 28,
  union_number      = 28,
  need_summon_count = 45,
  sell_tab0         = 45,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 3,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 21,
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