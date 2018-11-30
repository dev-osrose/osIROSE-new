registerNpc(23, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 400,
  r_weapon          = 110,
  l_weapon          = 0,
  level             = 0,
  hp                = 13,
  attack            = 28,
  hit               = 67,
  def               = 65,
  res               = 39,
  avoid             = 28,
  attack_spd        = 30,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 21,
  drop_type         = 27,
  drop_money        = 110,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 3,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
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