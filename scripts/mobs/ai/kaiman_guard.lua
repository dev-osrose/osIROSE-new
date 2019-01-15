registerNpc(142, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 700,
  r_weapon          = 155,
  l_weapon          = 104,
  level             = 0,
  hp                = 60,
  attack            = 35,
  hit               = 245,
  def               = 159,
  res               = 224,
  avoid             = 133,
  attack_spd        = 48,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 319,
  drop_type         = 50,
  drop_money        = 166,
  drop_item         = 16,
  union_number      = 16,
  need_summon_count = 49,
  sell_tab0         = 49,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 260,
  hit_material_type = 2,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
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