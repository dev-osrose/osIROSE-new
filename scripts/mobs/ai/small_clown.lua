registerNpc(181, {
  walk_speed        = 0,
  run_speed         = 180,
  scale             = 700,
  r_weapon          = 95,
  l_weapon          = 0,
  level             = 0,
  hp                = 47,
  attack            = 28,
  hit               = 232,
  def               = 142,
  res               = 146,
  avoid             = 64,
  attack_spd        = 48,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 22,
  drop_type         = 50,
  drop_money        = 191,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 53,
  sell_tab0         = 53,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 160,
  hit_material_type = 4,
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