registerNpc(511, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 550,
  r_weapon          = 200,
  l_weapon          = 0,
  level             = 0,
  hp                = 143,
  attack            = 33,
  hit               = 604,
  def               = 411,
  res               = 524,
  avoid             = 393,
  attack_spd        = 135,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 294,
  drop_type         = 118,
  drop_money        = 437,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 50,
  sell_tab0         = 50,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 240,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 20,
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