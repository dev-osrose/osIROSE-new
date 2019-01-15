registerNpc(96, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 400,
  r_weapon          = 96,
  l_weapon          = 941,
  level             = 0,
  hp                = 26,
  attack            = 20,
  hit               = 121,
  def               = 102,
  res               = 70,
  avoid             = 34,
  attack_spd        = 50,
  is_magic_damage   = 85,
  ai_type           = 0,
  give_exp          = 28,
  drop_type         = 28,
  drop_money        = 146,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 63,
  sell_tab0         = 63,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2000,
  hit_material_type = 6,
  face_icon         = 0,
  summon_mob_type   = 0,
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