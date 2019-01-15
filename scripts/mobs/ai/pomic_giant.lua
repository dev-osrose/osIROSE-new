registerNpc(76, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 700,
  r_weapon          = 185,
  l_weapon          = 1012,
  level             = 0,
  hp                = 24,
  attack            = 75,
  hit               = 132,
  def               = 109,
  res               = 96,
  avoid             = 55,
  attack_spd        = 32,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 318,
  drop_type         = 182,
  drop_money        = 135,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 65,
  sell_tab0         = 65,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 3,
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