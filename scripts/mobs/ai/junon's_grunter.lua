registerNpc(402, {
  walk_speed        = 260,
  run_speed         = 670,
  scale             = 330,
  r_weapon          = 138,
  l_weapon          = 0,
  level             = 66,
  hp                = 211,
  attack            = 352,
  hit               = 223,
  def               = 241,
  res               = 147,
  avoid             = 103,
  attack_spd        = 110,
  is_magic_damage   = 0,
  ai_type           = 72,
  give_exp          = 637,
  drop_type         = 302,
  drop_money        = 1,
  drop_item         = 90,
  union_number      = 90,
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