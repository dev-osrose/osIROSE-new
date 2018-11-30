registerNpc(91, {
  walk_speed        = 0,
  run_speed         = 130,
  scale             = 400,
  r_weapon          = 90,
  l_weapon          = 1011,
  level             = 0,
  hp                = 24,
  attack            = 25,
  hit               = 109,
  def               = 87,
  res               = 73,
  avoid             = 44,
  attack_spd        = 43,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 106,
  drop_type         = 31,
  drop_money        = 142,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 1,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 12,
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