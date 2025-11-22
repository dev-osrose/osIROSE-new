registerNpc(540, {
  walk_speed        = 450,
  run_speed         = 950,
  scale             = 130,
  r_weapon          = 1107,
  l_weapon          = 0,
  level             = 178,
  hp                = 43,
  attack            = 815,
  hit               = 532,
  def               = 610,
  res               = 494,
  avoid             = 219,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 292,
  give_exp          = 117,
  drop_type         = 458,
  drop_money        = 30,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 340,
  npc_type          = 3,
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