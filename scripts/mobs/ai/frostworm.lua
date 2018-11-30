registerNpc(361, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 750,
  r_weapon          = 200,
  l_weapon          = 1094,
  level             = 0,
  hp                = 115,
  attack            = 42,
  hit               = 493,
  def               = 273,
  res               = 830,
  avoid             = 296,
  attack_spd        = 103,
  is_magic_damage   = 90,
  ai_type           = 1,
  give_exp          = 335,
  drop_type         = 108,
  drop_money        = 393,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 53,
  sell_tab0         = 53,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 2,
  face_icon         = 2,
  summon_mob_type   = 2,
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