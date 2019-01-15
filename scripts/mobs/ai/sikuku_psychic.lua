registerNpc(554, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 490,
  r_weapon          = 100,
  l_weapon          = 1102,
  level             = 0,
  hp                = 160,
  attack            = 31,
  hit               = 772,
  def               = 474,
  res               = 422,
  avoid             = 783,
  attack_spd        = 258,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 295,
  drop_type         = 191,
  drop_money        = 473,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 64,
  sell_tab0         = 64,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1600,
  hit_material_type = 7,
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