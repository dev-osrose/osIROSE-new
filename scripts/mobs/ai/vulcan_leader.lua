registerNpc(366, {
  walk_speed        = 0,
  run_speed         = 280,
  scale             = 850,
  r_weapon          = 200,
  l_weapon          = 0,
  level             = 0,
  hp                = 86,
  attack            = 40,
  hit               = 429,
  def               = 215,
  res               = 330,
  avoid             = 143,
  attack_spd        = 136,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 144,
  drop_type         = 120,
  drop_money        = 365,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 50,
  sell_tab0         = 50,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 8,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 17,
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