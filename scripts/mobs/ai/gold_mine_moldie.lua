registerNpc(113, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 450,
  r_weapon          = 102,
  l_weapon          = 1016,
  level             = 0,
  hp                = 39,
  attack            = 27,
  hit               = 192,
  def               = 128,
  res               = 122,
  avoid             = 51,
  attack_spd        = 40,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 107,
  drop_type         = 36,
  drop_money        = 152,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 52,
  sell_tab0         = 52,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 4,
  face_icon         = 1,
  summon_mob_type   = 1,
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