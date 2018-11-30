registerNpc(512, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 550,
  r_weapon          = 250,
  l_weapon          = 0,
  level             = 0,
  hp                = 153,
  attack            = 38,
  hit               = 655,
  def               = 440,
  res               = 570,
  avoid             = 430,
  attack_spd        = 147,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 294,
  drop_type         = 132,
  drop_money        = 438,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 240,
  hit_material_type = 2,
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