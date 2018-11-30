registerNpc(69, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 500,
  r_weapon          = 135,
  l_weapon          = 333,
  level             = 0,
  hp                = 33,
  attack            = 43,
  hit               = 170,
  def               = 117,
  res               = 109,
  avoid             = 68,
  attack_spd        = 42,
  is_magic_damage   = 90,
  ai_type           = 1,
  give_exp          = 28,
  drop_type         = 81,
  drop_money        = 130,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 67,
  sell_tab0         = 67,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 9,
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