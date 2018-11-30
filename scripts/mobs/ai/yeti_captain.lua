registerNpc(344, {
  walk_speed        = 0,
  run_speed         = 235,
  scale             = 750,
  r_weapon          = 170,
  l_weapon          = 1009,
  level             = 0,
  hp                = 96,
  attack            = 41,
  hit               = 484,
  def               = 237,
  res               = 422,
  avoid             = 165,
  attack_spd        = 153,
  is_magic_damage   = 120,
  ai_type           = 0,
  give_exp          = 127,
  drop_type         = 144,
  drop_money        = 378,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 61,
  sell_tab0         = 61,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 8,
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