registerNpc(202, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 400,
  r_weapon          = 180,
  l_weapon          = 1084,
  level             = 0,
  hp                = 78,
  attack            = 32,
  hit               = 391,
  def               = 204,
  res               = 251,
  avoid             = 121,
  attack_spd        = 82,
  is_magic_damage   = 95,
  ai_type           = 1,
  give_exp          = 52,
  drop_type         = 56,
  drop_money        = 197,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1200,
  hit_material_type = 4,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 18,
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