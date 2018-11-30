registerNpc(280, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 850,
  r_weapon          = 160,
  l_weapon          = 39,
  level             = 3,
  hp                = 60,
  attack            = 46,
  hit               = 309,
  def               = 167,
  res               = 193,
  avoid             = 125,
  attack_spd        = 73,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 117,
  drop_type         = 75,
  drop_money        = 235,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 57,
  sell_tab0         = 57,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 9,
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