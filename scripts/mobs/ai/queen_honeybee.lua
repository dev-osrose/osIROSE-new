registerNpc(67, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 320,
  r_weapon          = 95,
  l_weapon          = 1061,
  level             = 0,
  hp                = 26,
  attack            = 20,
  hit               = 107,
  def               = 90,
  res               = 51,
  avoid             = 100,
  attack_spd        = 45,
  is_magic_damage   = 80,
  ai_type           = 1,
  give_exp          = 28,
  drop_type         = 30,
  drop_money        = 128,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 800,
  hit_material_type = 7,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
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