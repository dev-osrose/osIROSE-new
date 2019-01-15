registerNpc(315, {
  walk_speed        = 0,
  run_speed         = 100,
  scale             = 250,
  r_weapon          = 100,
  l_weapon          = 0,
  level             = 0,
  hp                = 1,
  attack            = 94,
  hit               = 20,
  def               = 70,
  res               = 500,
  avoid             = 500,
  attack_spd        = 500,
  is_magic_damage   = 10,
  ai_type           = 0,
  give_exp          = 91,
  drop_type         = 4,
  drop_money        = 8,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 1,
  sell_tab0         = 1,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 1,
  npc_type          = 100,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 19,
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