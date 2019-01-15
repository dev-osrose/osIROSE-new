registerNpc(549, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 650,
  r_weapon          = 100,
  l_weapon          = 1111,
  level             = 0,
  hp                = 180,
  attack            = 36,
  hit               = 891,
  def               = 616,
  res               = 489,
  avoid             = 542,
  attack_spd        = 358,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 291,
  drop_type         = 144,
  drop_money        = 468,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 2000,
  hit_material_type = 6,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 21,
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