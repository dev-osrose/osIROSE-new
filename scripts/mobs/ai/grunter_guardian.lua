registerNpc(216, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 320,
  l_weapon          = 138,
  level             = 0,
  hp                = 70,
  attack            = 375,
  hit               = 387,
  def               = 289,
  res               = 266,
  avoid             = 177,
  attack_spd        = 109,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 257,
  drop_type         = 1764,
  drop_money        = 272,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 82,
  sell_tab0         = 82,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 15,
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