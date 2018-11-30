registerNpc(88, {
  walk_speed        = 0,
  run_speed         = 130,
  scale             = 320,
  r_weapon          = 75,
  l_weapon          = 0,
  level             = 0,
  hp                = 22,
  attack            = 27,
  hit               = 106,
  def               = 78,
  res               = 57,
  avoid             = 43,
  attack_spd        = 40,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 105,
  drop_type         = 30,
  drop_money        = 141,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 55,
  sell_tab0         = 55,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 3,
  face_icon         = 2,
  summon_mob_type   = 2,
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