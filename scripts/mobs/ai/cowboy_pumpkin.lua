registerNpc(24, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 260,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 14,
  attack            = 25,
  hit               = 75,
  def               = 85,
  res               = 56,
  avoid             = 18,
  attack_spd        = 18,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 23,
  drop_type         = 27,
  drop_money        = 111,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 65,
  sell_tab0         = 65,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 4,
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