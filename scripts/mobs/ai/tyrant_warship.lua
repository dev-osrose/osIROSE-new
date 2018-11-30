registerNpc(356, {
  walk_speed        = 0,
  run_speed         = 300,
  scale             = 950,
  r_weapon          = 250,
  l_weapon          = 1093,
  level             = 0,
  hp                = 114,
  attack            = 88,
  hit               = 644,
  def               = 303,
  res               = 783,
  avoid             = 286,
  attack_spd        = 147,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 134,
  drop_type         = 384,
  drop_money        = 392,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 73,
  sell_tab0         = 73,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1700,
  hit_material_type = 10,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 16,
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