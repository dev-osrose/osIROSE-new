registerNpc(145, {
  walk_speed        = 0,
  run_speed         = 210,
  scale             = 700,
  r_weapon          = 170,
  l_weapon          = 1045,
  level             = 0,
  hp                = 63,
  attack            = 24,
  hit               = 290,
  def               = 169,
  res               = 170,
  avoid             = 93,
  attack_spd        = 103,
  is_magic_damage   = 85,
  ai_type           = 0,
  give_exp          = 322,
  drop_type         = 46,
  drop_money        = 169,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 55,
  sell_tab0         = 55,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2000,
  hit_material_type = 6,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
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