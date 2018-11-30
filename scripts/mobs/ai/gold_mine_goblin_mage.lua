registerNpc(283, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 750,
  r_weapon          = 150,
  l_weapon          = 337,
  level             = 0,
  hp                = 76,
  attack            = 48,
  hit               = 396,
  def               = 200,
  res               = 249,
  avoid             = 164,
  attack_spd        = 93,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 119,
  drop_type         = 75,
  drop_money        = 238,
  drop_item         = 10,
  union_number      = 10,
  need_summon_count = 58,
  sell_tab0         = 58,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1000,
  hit_material_type = 9,
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