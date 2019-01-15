registerNpc(929, {
  walk_speed        = 0,
  run_speed         = 210,
  scale             = 470,
  r_weapon          = 140,
  l_weapon          = 38,
  level             = 2,
  hp                = 75,
  attack            = 3036,
  hit               = 256,
  def               = 176,
  res               = 248,
  avoid             = 97,
  attack_spd        = 91,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 166,
  drop_type         = 0,
  drop_money        = 3,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 100,
  sell_tab0         = 100,
  sell_tab1         = 35,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
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