registerNpc(185, {
  walk_speed        = 0,
  run_speed         = 175,
  scale             = 700,
  r_weapon          = 95,
  l_weapon          = 203,
  level             = 0,
  hp                = 48,
  attack            = 26,
  hit               = 200,
  def               = 122,
  res               = 101,
  avoid             = 106,
  attack_spd        = 93,
  is_magic_damage   = 102,
  ai_type           = 0,
  give_exp          = 321,
  drop_type         = 46,
  drop_money        = 194,
  drop_item         = 22,
  union_number      = 22,
  need_summon_count = 53,
  sell_tab0         = 53,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2500,
  hit_material_type = 5,
  face_icon         = 1,
  summon_mob_type   = 1,
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