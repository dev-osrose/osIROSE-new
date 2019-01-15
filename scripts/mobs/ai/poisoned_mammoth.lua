registerNpc(377, {
  walk_speed        = 0,
  run_speed         = 310,
  scale             = 1050,
  r_weapon          = 250,
  l_weapon          = 0,
  level             = 0,
  hp                = 85,
  attack            = 35,
  hit               = 406,
  def               = 181,
  res               = 271,
  avoid             = 187,
  attack_spd        = 127,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 136,
  drop_type         = 77,
  drop_money        = 364,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 53,
  sell_tab0         = 53,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 3,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 17,
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