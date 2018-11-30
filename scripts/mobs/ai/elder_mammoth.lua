registerNpc(379, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 1050,
  r_weapon          = 350,
  l_weapon          = 0,
  level             = 0,
  hp                = 88,
  attack            = 50,
  hit               = 464,
  def               = 225,
  res               = 353,
  avoid             = 194,
  attack_spd        = 110,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 136,
  drop_type         = 141,
  drop_money        = 370,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 73,
  sell_tab0         = 73,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 9,
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