registerNpc(171, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 700,
  r_weapon          = 110,
  l_weapon          = 1035,
  level             = 0,
  hp                = 30,
  attack            = 32,
  hit               = 123,
  def               = 105,
  res               = 120,
  avoid             = 64,
  attack_spd        = 24,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 319,
  drop_type         = 36,
  drop_money        = 186,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 61,
  sell_tab0         = 61,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 2,
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