registerNpc(408, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 500,
  r_weapon          = 460,
  l_weapon          = 1084,
  level             = 0,
  hp                = 95,
  attack            = 282,
  hit               = 530,
  def               = 334,
  res               = 475,
  avoid             = 235,
  attack_spd        = 163,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 76,
  drop_type         = 1320,
  drop_money        = 308,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 70,
  sell_tab0         = 70,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1200,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
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