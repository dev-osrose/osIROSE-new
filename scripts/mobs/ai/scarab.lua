registerNpc(210, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 500,
  r_weapon          = 50,
  l_weapon          = 0,
  level             = 0,
  hp                = 35,
  attack            = 26,
  hit               = 173,
  def               = 121,
  res               = 110,
  avoid             = 45,
  attack_spd        = 37,
  is_magic_damage   = 130,
  ai_type           = 0,
  give_exp          = 58,
  drop_type         = 40,
  drop_money        = 205,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 20,
  sell_tab0         = 20,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 130,
  hit_material_type = 4,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 18,
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