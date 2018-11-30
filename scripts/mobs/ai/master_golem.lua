registerNpc(163, {
  walk_speed        = 0,
  run_speed         = 320,
  scale             = 700,
  r_weapon          = 310,
  l_weapon          = 0,
  level             = 0,
  hp                = 84,
  attack            = 181,
  hit               = 470,
  def               = 260,
  res               = 266,
  avoid             = 216,
  attack_spd        = 133,
  is_magic_damage   = 85,
  ai_type           = 0,
  give_exp          = 64,
  drop_type         = 631,
  drop_money        = 181,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 80,
  sell_tab0         = 80,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 2,
  summon_mob_type   = 2,
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