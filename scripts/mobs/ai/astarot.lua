registerNpc(395, {
  walk_speed        = 300,
  run_speed         = 710,
  scale             = 370,
  r_weapon          = 1055,
  l_weapon          = 0,
  level             = 145,
  hp                = 448,
  attack            = 860,
  hit               = 536,
  def               = 1173,
  res               = 529,
  avoid             = 219,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 270,
  give_exp          = 2530,
  drop_type         = 277,
  drop_money        = 0,
  drop_item         = 85,
  union_number      = 85,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 10,
  hit_material_type = 2,
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