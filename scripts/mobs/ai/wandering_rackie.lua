registerNpc(204, {
  walk_speed        = 0,
  run_speed         = 160,
  scale             = 540,
  r_weapon          = 105,
  l_weapon          = 1011,
  level             = 0,
  hp                = 20,
  attack            = 34,
  hit               = 101,
  def               = 94,
  res               = 71,
  avoid             = 28,
  attack_spd        = 42,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 8,
  drop_type         = 51,
  drop_money        = 199,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 8,
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