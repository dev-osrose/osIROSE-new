registerNpc(373, {
  walk_speed        = 0,
  run_speed         = 260,
  scale             = 700,
  r_weapon          = 130,
  l_weapon          = 0,
  level             = 0,
  hp                = 90,
  attack            = 33,
  hit               = 414,
  def               = 203,
  res               = 375,
  avoid             = 196,
  attack_spd        = 135,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 122,
  drop_type         = 78,
  drop_money        = 371,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 51,
  sell_tab0         = 51,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 800,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 17,
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