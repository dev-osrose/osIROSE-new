registerNpc(218, {
  walk_speed        = 0,
  run_speed         = 320,
  scale             = 690,
  r_weapon          = 360,
  l_weapon          = 0,
  level             = 0,
  hp                = 95,
  attack            = 412,
  hit               = 537,
  def               = 376,
  res               = 502,
  avoid             = 246,
  attack_spd        = 163,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 259,
  drop_type         = 2182,
  drop_money        = 274,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 84,
  sell_tab0         = 84,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 15,
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