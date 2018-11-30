registerNpc(346, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 700,
  r_weapon          = 140,
  l_weapon          = 1064,
  level             = 0,
  hp                = 95,
  attack            = 27,
  hit               = 399,
  def               = 213,
  res               = 301,
  avoid             = 346,
  attack_spd        = 143,
  is_magic_damage   = 95,
  ai_type           = 1,
  give_exp          = 128,
  drop_type         = 77,
  drop_money        = 377,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 48,
  sell_tab0         = 48,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1200,
  hit_material_type = 7,
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