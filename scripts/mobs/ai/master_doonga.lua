registerNpc(159, {
  walk_speed        = 0,
  run_speed         = 260,
  scale             = 700,
  r_weapon          = 250,
  l_weapon          = 0,
  level             = 0,
  hp                = 60,
  attack            = 175,
  hit               = 330,
  def               = 196,
  res               = 227,
  avoid             = 161,
  attack_spd        = 73,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 60,
  drop_type         = 557,
  drop_money        = 178,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 72,
  sell_tab0         = 72,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
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