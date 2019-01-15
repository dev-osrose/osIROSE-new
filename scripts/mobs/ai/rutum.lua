registerNpc(382, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 1100,
  r_weapon          = 280,
  l_weapon          = 0,
  level             = 0,
  hp                = 110,
  attack            = 210,
  hit               = 631,
  def               = 310,
  res               = 780,
  avoid             = 327,
  attack_spd        = 141,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 337,
  drop_type         = 914,
  drop_money        = 390,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 80,
  sell_tab0         = 80,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 11,
  face_icon         = 2,
  summon_mob_type   = 2,
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