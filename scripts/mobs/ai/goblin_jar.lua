registerNpc(261, {
  walk_speed        = 0,
  run_speed         = 150,
  scale             = 350,
  r_weapon          = 100,
  l_weapon          = 0,
  level             = 0,
  hp                = 45,
  attack            = 33,
  hit               = 183,
  def               = 131,
  res               = 169,
  avoid             = 96,
  attack_spd        = 36,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 1,
  drop_type         = 27,
  drop_money        = 224,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 52,
  sell_tab0         = 52,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 800,
  hit_material_type = 2,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 14,
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