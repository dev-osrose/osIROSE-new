registerNpc(540, {
  walk_speed        = 0,
  run_speed         = 450,
  scale             = 950,
  r_weapon          = 130,
  l_weapon          = 1107,
  level             = 0,
  hp                = 178,
  attack            = 43,
  hit               = 815,
  def               = 532,
  res               = 610,
  avoid             = 494,
  attack_spd        = 219,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 292,
  drop_type         = 117,
  drop_money        = 458,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 40,
  sell_tab0         = 40,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 340,
  hit_material_type = 3,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 21,
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