registerNpc(273, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 850,
  r_weapon          = 140,
  l_weapon          = 1016,
  level             = 0,
  hp                = 65,
  attack            = 33,
  hit               = 306,
  def               = 145,
  res               = 165,
  avoid             = 135,
  attack_spd        = 97,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 328,
  drop_type         = 48,
  drop_money        = 228,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 46,
  sell_tab0         = 46,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
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