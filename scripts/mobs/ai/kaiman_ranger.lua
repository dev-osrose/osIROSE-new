registerNpc(144, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 700,
  r_weapon          = 152,
  l_weapon          = 1045,
  level             = 0,
  hp                = 61,
  attack            = 28,
  hit               = 255,
  def               = 144,
  res               = 133,
  avoid             = 139,
  attack_spd        = 115,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 321,
  drop_type         = 54,
  drop_money        = 168,
  drop_item         = 16,
  union_number      = 16,
  need_summon_count = 53,
  sell_tab0         = 53,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2400,
  hit_material_type = 5,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 13,
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