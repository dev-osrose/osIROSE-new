registerNpc(630, {
  walk_speed        = 0,
  run_speed         = 500,
  scale             = 1050,
  r_weapon          = 450,
  l_weapon          = 0,
  level             = 0,
  hp                = 90,
  attack            = 197,
  hit               = 506,
  def               = 260,
  res               = 472,
  avoid             = 256,
  attack_spd        = 112,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 276,
  drop_type         = 900,
  drop_money        = 334,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 19,
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