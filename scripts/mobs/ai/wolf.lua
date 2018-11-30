registerNpc(336, {
  walk_speed        = 0,
  run_speed         = 215,
  scale             = 650,
  r_weapon          = 110,
  l_weapon          = 0,
  level             = 0,
  hp                = 76,
  attack            = 34,
  hit               = 361,
  def               = 165,
  res               = 197,
  avoid             = 163,
  attack_spd        = 114,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 121,
  drop_type         = 60,
  drop_money        = 357,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 46,
  sell_tab0         = 46,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 3,
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