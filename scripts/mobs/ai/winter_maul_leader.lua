registerNpc(369, {
  walk_speed        = 290,
  run_speed         = 800,
  scale             = 200,
  r_weapon          = 0,
  l_weapon          = 0,
  level             = 108,
  hp                = 42,
  attack            = 550,
  hit               = 263,
  def               = 631,
  res               = 192,
  avoid             = 173,
  attack_spd        = 115,
  is_magic_damage   = 0,
  ai_type           = 144,
  give_exp          = 142,
  drop_type         = 385,
  drop_money        = 0,
  drop_item         = 58,
  union_number      = 58,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 250,
  npc_type          = 8,
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