registerNpc(203, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 360,
  r_weapon          = 170,
  l_weapon          = 0,
  level             = 0,
  hp                = 13,
  attack            = 49,
  hit               = 74,
  def               = 84,
  res               = 57,
  avoid             = 34,
  attack_spd        = 21,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 54,
  drop_type         = 64,
  drop_money        = 198,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 230,
  hit_material_type = 9,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 15,
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