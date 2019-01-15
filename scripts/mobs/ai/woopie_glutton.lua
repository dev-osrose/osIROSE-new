registerNpc(103, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 700,
  r_weapon          = 110,
  l_weapon          = 1003,
  level             = 302,
  hp                = 18,
  attack            = 34,
  hit               = 76,
  def               = 89,
  res               = 66,
  avoid             = 26,
  attack_spd        = 39,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 23,
  drop_type         = 40,
  drop_money        = 149,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 65,
  sell_tab0         = 65,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 8,
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