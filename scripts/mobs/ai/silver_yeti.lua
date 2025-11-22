registerNpc(391, {
  walk_speed        = 220,
  run_speed         = 550,
  scale             = 180,
  r_weapon          = 1008,
  l_weapon          = 0,
  level             = 99,
  hp                = 135,
  attack            = 562,
  hit               = 331,
  def               = 526,
  res               = 213,
  avoid             = 154,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 124,
  give_exp          = 420,
  drop_type         = 382,
  drop_money        = 0,
  drop_item         = 99,
  union_number      = 99,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 10,
  hit_material_type = 1,
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