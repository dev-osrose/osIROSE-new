registerNpc(401, {
  walk_speed        = 220,
  run_speed         = 650,
  scale             = 260,
  r_weapon          = 167,
  l_weapon          = 0,
  level             = 47,
  hp                = 194,
  attack            = 265,
  hit               = 180,
  def               = 180,
  res               = 116,
  avoid             = 79,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 71,
  give_exp          = 733,
  drop_type         = 301,
  drop_money        = 1,
  drop_item         = 85,
  union_number      = 85,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 350,
  npc_type          = 10,
  hit_material_type = 0,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 1,
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