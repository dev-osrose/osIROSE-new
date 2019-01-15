registerNpc(352, {
  walk_speed        = 0,
  run_speed         = 275,
  scale             = 800,
  r_weapon          = 190,
  l_weapon          = 1092,
  level             = 0,
  hp                = 108,
  attack            = 28,
  hit               = 460,
  def               = 239,
  res               = 462,
  avoid             = 402,
  attack_spd        = 165,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 130,
  drop_type         = 98,
  drop_money        = 387,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 49,
  sell_tab0         = 49,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 7,
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