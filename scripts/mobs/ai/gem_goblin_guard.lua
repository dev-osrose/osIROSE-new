registerNpc(277, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 850,
  r_weapon          = 155,
  l_weapon          = 40,
  level             = 3,
  hp                = 85,
  attack            = 39,
  hit               = 354,
  def               = 208,
  res               = 389,
  avoid             = 202,
  attack_spd        = 72,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 116,
  drop_type         = 54,
  drop_money        = 232,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 40,
  sell_tab0         = 40,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 14,
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