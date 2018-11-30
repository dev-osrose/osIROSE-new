registerNpc(1, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 370,
  r_weapon          = 56,
  l_weapon          = 0,
  level             = 0,
  hp                = 2,
  attack            = 15,
  hit               = 2,
  def               = 56,
  res               = 25,
  avoid             = 11,
  attack_spd        = 5,
  is_magic_damage   = 80,
  ai_type           = 0,
  give_exp          = 101,
  drop_type         = 6,
  drop_money        = 100,
  drop_item         = 6,
  union_number      = 6,
  need_summon_count = 70,
  sell_tab0         = 70,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 190,
  hit_material_type = 1,
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