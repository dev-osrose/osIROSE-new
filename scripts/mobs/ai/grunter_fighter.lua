registerNpc(135, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 700,
  r_weapon          = 120,
  l_weapon          = 134,
  level             = 0,
  hp                = 44,
  attack            = 30,
  hit               = 205,
  def               = 111,
  res               = 108,
  avoid             = 86,
  attack_spd        = 68,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 115,
  drop_type         = 43,
  drop_money        = 161,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 65,
  sell_tab0         = 65,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 3,
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