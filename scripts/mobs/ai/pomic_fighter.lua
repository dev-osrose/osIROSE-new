registerNpc(75, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 700,
  r_weapon          = 102,
  l_weapon          = 1032,
  level             = 0,
  hp                = 19,
  attack            = 27,
  hit               = 93,
  def               = 74,
  res               = 51,
  avoid             = 37,
  attack_spd        = 39,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 5,
  drop_type         = 28,
  drop_money        = 135,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 65,
  sell_tab0         = 65,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 320,
  hit_material_type = 3,
  face_icon         = 0,
  summon_mob_type   = 0,
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