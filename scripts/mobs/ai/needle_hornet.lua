registerNpc(63, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 400,
  r_weapon          = 70,
  l_weapon          = 0,
  level             = 0,
  hp                = 15,
  attack            = 28,
  hit               = 75,
  def               = 68,
  res               = 43,
  avoid             = 31,
  attack_spd        = 32,
  is_magic_damage   = 87,
  ai_type           = 0,
  give_exp          = 5,
  drop_type         = 27,
  drop_money        = 126,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 64,
  sell_tab0         = 64,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 3,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 11,
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