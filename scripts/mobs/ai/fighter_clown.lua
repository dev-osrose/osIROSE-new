registerNpc(183, {
  walk_speed        = 0,
  run_speed         = 170,
  scale             = 700,
  r_weapon          = 115,
  l_weapon          = 1015,
  level             = 0,
  hp                = 52,
  attack            = 31,
  hit               = 243,
  def               = 124,
  res               = 128,
  avoid             = 104,
  attack_spd        = 79,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 320,
  drop_type         = 53,
  drop_money        = 193,
  drop_item         = 28,
  union_number      = 28,
  need_summon_count = 56,
  sell_tab0         = 56,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 3,
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