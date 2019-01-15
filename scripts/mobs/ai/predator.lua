registerNpc(529, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 300,
  l_weapon          = 0,
  level             = 0,
  hp                = 158,
  attack            = 42,
  hit               = 821,
  def               = 455,
  res               = 497,
  avoid             = 402,
  attack_spd        = 270,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 296,
  drop_type         = 140,
  drop_money        = 451,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 3,
  face_icon         = 0,
  summon_mob_type   = 0,
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