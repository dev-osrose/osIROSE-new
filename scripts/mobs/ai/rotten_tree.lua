registerNpc(571, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 200,
  l_weapon          = 1082,
  level             = 0,
  hp                = 147,
  attack            = 29,
  hit               = 697,
  def               = 449,
  res               = 501,
  avoid             = 469,
  attack_spd        = 271,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 303,
  drop_type         = 124,
  drop_money        = 485,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 25,
  sell_tab0         = 25,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1300,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 20,
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