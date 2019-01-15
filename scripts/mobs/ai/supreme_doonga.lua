registerNpc(763, {
  walk_speed        = 0,
  run_speed         = 450,
  scale             = 750,
  r_weapon          = 432,
  l_weapon          = 0,
  level             = 0,
  hp                = 99,
  attack            = 34,
  hit               = 389,
  def               = 221,
  res               = 312,
  avoid             = 222,
  attack_spd        = 150,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 40,
  drop_type         = 28,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 18,
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