registerNpc(350, {
  walk_speed        = 0,
  run_speed         = 215,
  scale             = 700,
  r_weapon          = 200,
  l_weapon          = 1091,
  level             = 0,
  hp                = 108,
  attack            = 86,
  hit               = 607,
  def               = 288,
  res               = 717,
  avoid             = 268,
  attack_spd        = 138,
  is_magic_damage   = 135,
  ai_type           = 0,
  give_exp          = 334,
  drop_type         = 103,
  drop_money        = 386,
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