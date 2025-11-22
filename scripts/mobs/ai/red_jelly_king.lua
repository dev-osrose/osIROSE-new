registerNpc(628, {
  walk_speed        = 500,
  run_speed         = 1050,
  scale             = 350,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 50,
  hp                = 167,
  attack            = 274,
  hit               = 170,
  def               = 190,
  res               = 133,
  avoid             = 61,
  attack_spd        = 90,
  is_magic_damage   = 0,
  ai_type           = 276,
  give_exp          = 705,
  drop_type         = 332,
  drop_money        = 12,
  drop_item         = 60,
  union_number      = 60,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 300,
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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