registerNpc(275, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 800,
  r_weapon          = 135,
  l_weapon          = 1080,
  level             = 0,
  hp                = 67,
  attack            = 33,
  hit               = 316,
  def               = 149,
  res               = 170,
  avoid             = 140,
  attack_spd        = 100,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 12,
  drop_type         = 46,
  drop_money        = 230,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 44,
  sell_tab0         = 44,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 900,
  hit_material_type = 3,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 14,
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