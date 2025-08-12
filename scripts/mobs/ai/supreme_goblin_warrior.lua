registerNpc(760, {
  walk_speed        = 225,
  run_speed         = 850,
  scale             = 180,
  r_weapon          = 169,
  l_weapon          = 0,
  level             = 101,
  hp                = 35,
  attack            = 398,
  hit               = 225,
  def               = 320,
  res               = 228,
  avoid             = 153,
  attack_spd        = 100,
  is_magic_damage   = 0,
  ai_type           = 40,
  give_exp          = 30,
  drop_type         = 0,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 1,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
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