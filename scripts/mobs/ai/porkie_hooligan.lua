registerNpc(133, {
  walk_speed        = 0,
  run_speed         = 185,
  scale             = 700,
  r_weapon          = 120,
  l_weapon          = 0,
  level             = 0,
  hp                = 35,
  attack            = 26,
  hit               = 173,
  def               = 121,
  res               = 110,
  avoid             = 45,
  attack_spd        = 37,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 5,
  drop_type         = 42,
  drop_money        = 159,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 63,
  sell_tab0         = 63,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 210,
  hit_material_type = 4,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
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