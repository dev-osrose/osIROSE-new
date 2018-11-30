registerNpc(274, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 750,
  r_weapon          = 108,
  l_weapon          = 1080,
  level             = 0,
  hp                = 52,
  attack            = 28,
  hit               = 232,
  def               = 132,
  res               = 150,
  avoid             = 100,
  attack_spd        = 79,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 11,
  drop_type         = 39,
  drop_money        = 229,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 57,
  sell_tab0         = 57,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 800,
  hit_material_type = 1,
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