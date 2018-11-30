registerNpc(286, {
  walk_speed        = 0,
  run_speed         = 310,
  scale             = 850,
  r_weapon          = 280,
  l_weapon          = 173,
  level             = 0,
  hp                = 100,
  attack            = 204,
  hit               = 568,
  def               = 303,
  res               = 532,
  avoid             = 291,
  attack_spd        = 127,
  is_magic_damage   = 105,
  ai_type           = 0,
  give_exp          = 69,
  drop_type         = 710,
  drop_money        = 240,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 76,
  sell_tab0         = 76,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
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