registerNpc(143, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 700,
  r_weapon          = 170,
  l_weapon          = 105,
  level             = 0,
  hp                = 64,
  attack            = 30,
  hit               = 318,
  def               = 175,
  res               = 201,
  avoid             = 94,
  attack_spd        = 66,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 320,
  drop_type         = 50,
  drop_money        = 167,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 52,
  sell_tab0         = 52,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 280,
  hit_material_type = 4,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
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