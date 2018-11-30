registerNpc(628, {
  walk_speed        = 0,
  run_speed         = 500,
  scale             = 1050,
  r_weapon          = 350,
  l_weapon          = 0,
  level             = 0,
  hp                = 50,
  attack            = 167,
  hit               = 274,
  def               = 170,
  res               = 190,
  avoid             = 133,
  attack_spd        = 61,
  is_magic_damage   = 90,
  ai_type           = 0,
  give_exp          = 276,
  drop_type         = 705,
  drop_money        = 332,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 300,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 19,
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