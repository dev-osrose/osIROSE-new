registerNpc(353, {
  walk_speed        = 0,
  run_speed         = 290,
  scale             = 850,
  r_weapon          = 190,
  l_weapon          = 1036,
  level             = 0,
  hp                = 110,
  attack            = 53,
  hit               = 592,
  def               = 274,
  res               = 679,
  avoid             = 255,
  attack_spd        = 141,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 132,
  drop_type         = 226,
  drop_money        = 388,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 63,
  sell_tab0         = 63,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 9,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 16,
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