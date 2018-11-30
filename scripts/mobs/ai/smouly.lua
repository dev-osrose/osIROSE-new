registerNpc(51, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 314,
  r_weapon          = 100,
  l_weapon          = 1074,
  level             = 0,
  hp                = 44,
  attack            = 27,
  hit               = 196,
  def               = 119,
  res               = 126,
  avoid             = 83,
  attack_spd        = 68,
  is_magic_damage   = 95,
  ai_type           = 1,
  give_exp          = 26,
  drop_type         = 40,
  drop_money        = 121,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 56,
  sell_tab0         = 56,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1200,
  hit_material_type = 1,
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