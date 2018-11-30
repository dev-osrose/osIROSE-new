registerNpc(175, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 700,
  r_weapon          = 112,
  l_weapon          = 1043,
  level             = 0,
  hp                = 32,
  attack            = 24,
  hit               = 134,
  def               = 96,
  res               = 66,
  avoid             = 70,
  attack_spd        = 68,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 321,
  drop_type         = 38,
  drop_money        = 189,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2200,
  hit_material_type = 5,
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