registerNpc(294, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 500,
  r_weapon          = 90,
  l_weapon          = 1096,
  level             = 0,
  hp                = 50,
  attack            = 350,
  hit               = 1,
  def               = 10,
  res               = 100,
  avoid             = 50,
  attack_spd        = 60,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 165,
  drop_type         = 1,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1600,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 19,
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