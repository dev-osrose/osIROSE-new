registerNpc(335, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 1050,
  r_weapon          = 250,
  l_weapon          = 0,
  level             = 0,
  hp                = 83,
  attack            = 81,
  hit               = 456,
  def               = 229,
  res               = 362,
  avoid             = 194,
  attack_spd        = 103,
  is_magic_damage   = 200,
  ai_type           = 0,
  give_exp          = 333,
  drop_type         = 84,
  drop_money        = 362,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 70,
  sell_tab0         = 70,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
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