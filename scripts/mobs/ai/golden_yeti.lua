registerNpc(392, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 560,
  r_weapon          = 190,
  l_weapon          = 1008,
  level             = 304,
  hp                = 105,
  attack            = 136,
  hit               = 599,
  def               = 349,
  res               = 724,
  avoid             = 228,
  attack_spd        = 164,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 126,
  drop_type         = 430,
  drop_money        = 383,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 99,
  sell_tab0         = 99,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 0,
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