registerNpc(215, {
  walk_speed        = 290,
  run_speed         = 630,
  scale             = 270,
  r_weapon          = 167,
  l_weapon          = 0,
  level             = 55,
  hp                = 352,
  attack            = 302,
  hit               = 241,
  def               = 205,
  res               = 143,
  avoid             = 89,
  attack_spd        = 125,
  is_magic_damage   = 0,
  ai_type           = 256,
  give_exp          = 1705,
  drop_type         = 271,
  drop_money        = 0,
  drop_item         = 80,
  union_number      = 80,
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
  quest_type        = 0,
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