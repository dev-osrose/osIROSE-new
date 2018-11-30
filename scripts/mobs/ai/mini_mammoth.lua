registerNpc(375, {
  walk_speed        = 0,
  run_speed         = 260,
  scale             = 850,
  r_weapon          = 150,
  l_weapon          = 0,
  level             = 0,
  hp                = 78,
  attack            = 32,
  hit               = 355,
  def               = 180,
  res               = 235,
  avoid             = 164,
  attack_spd        = 117,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 139,
  drop_type         = 64,
  drop_money        = 358,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 52,
  sell_tab0         = 52,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 1,
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