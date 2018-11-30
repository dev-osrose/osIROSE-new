registerNpc(328, {
  walk_speed        = 0,
  run_speed         = 0,
  scale             = 0,
  r_weapon          = 260,
  l_weapon          = 0,
  level             = 0,
  hp                = 80,
  attack            = 250,
  hit               = 360,
  def               = 247,
  res               = 222,
  avoid             = 234,
  attack_spd        = 72,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 95,
  drop_type         = 0,
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
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 16,
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