registerNpc(558, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 190,
  l_weapon          = 142,
  level             = 0,
  hp                = 175,
  attack            = 181,
  hit               = 953,
  def               = 502,
  res               = 730,
  avoid             = 607,
  attack_spd        = 432,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 298,
  drop_type         = 666,
  drop_money        = 477,
  drop_item         = 5,
  union_number      = 5,
  need_summon_count = 75,
  sell_tab0         = 75,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 270,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 22,
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