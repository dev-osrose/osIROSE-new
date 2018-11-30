registerNpc(85, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 470,
  r_weapon          = 85,
  l_weapon          = 0,
  level             = 0,
  hp                = 29,
  attack            = 32,
  hit               = 119,
  def               = 104,
  res               = 116,
  avoid             = 62,
  attack_spd        = 23,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 4,
  drop_type         = 36,
  drop_money        = 139,
  drop_item         = 23,
  union_number      = 23,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 2,
  face_icon         = 2,
  summon_mob_type   = 2,
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