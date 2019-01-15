registerNpc(543, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 130,
  l_weapon          = 1025,
  level             = 0,
  hp                = 158,
  attack            = 38,
  hit               = 852,
  def               = 468,
  res               = 497,
  avoid             = 420,
  attack_spd        = 254,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 297,
  drop_type         = 169,
  drop_money        = 462,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 40,
  sell_tab0         = 40,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 270,
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