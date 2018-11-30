registerNpc(362, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 1050,
  r_weapon          = 300,
  l_weapon          = 1094,
  level             = 0,
  hp                = 118,
  attack            = 39,
  hit               = 582,
  def               = 245,
  res               = 618,
  avoid             = 285,
  attack_spd        = 181,
  is_magic_damage   = 95,
  ai_type           = 1,
  give_exp          = 135,
  drop_type         = 132,
  drop_money        = 394,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 55,
  sell_tab0         = 55,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 3,
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