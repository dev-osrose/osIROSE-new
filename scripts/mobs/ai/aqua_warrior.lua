registerNpc(172, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 700,
  r_weapon          = 125,
  l_weapon          = 164,
  level             = 0,
  hp                = 31,
  attack            = 26,
  hit               = 154,
  def               = 114,
  res               = 99,
  avoid             = 39,
  attack_spd        = 33,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 320,
  drop_type         = 34,
  drop_money        = 187,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 63,
  sell_tab0         = 63,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 360,
  hit_material_type = 4,
  face_icon         = 0,
  summon_mob_type   = 0,
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