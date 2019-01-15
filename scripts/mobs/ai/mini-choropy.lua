registerNpc(11, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 242,
  r_weapon          = 35,
  l_weapon          = 0,
  level             = 0,
  hp                = 4,
  attack            = 15,
  hit               = 6,
  def               = 59,
  res               = 29,
  avoid             = 13,
  attack_spd        = 6,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 1,
  drop_type         = 9,
  drop_money        = 104,
  drop_item         = 5,
  union_number      = 5,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 180,
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