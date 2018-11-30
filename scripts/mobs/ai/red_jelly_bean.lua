registerNpc(5, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 450,
  r_weapon          = 55,
  l_weapon          = 0,
  level             = 0,
  hp                = 6,
  attack            = 11,
  hit               = 12,
  def               = 56,
  res               = 33,
  avoid             = 17,
  attack_spd        = 15,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 104,
  drop_type         = 11,
  drop_money        = 103,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 75,
  sell_tab0         = 75,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 3,
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