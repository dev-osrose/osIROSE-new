registerNpc(93, {
  walk_speed        = 0,
  run_speed         = 145,
  scale             = 600,
  r_weapon          = 100,
  l_weapon          = 1005,
  level             = 301,
  hp                = 28,
  attack            = 28,
  hit               = 132,
  def               = 87,
  res               = 70,
  avoid             = 53,
  attack_spd        = 48,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 23,
  drop_type         = 33,
  drop_money        = 144,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 62,
  sell_tab0         = 62,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 3,
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