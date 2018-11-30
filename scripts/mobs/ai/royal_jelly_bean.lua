registerNpc(4, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 490,
  r_weapon          = 45,
  l_weapon          = 0,
  level             = 0,
  hp                = 4,
  attack            = 10,
  hit               = 7,
  def               = 78,
  res               = 29,
  avoid             = 5,
  attack_spd        = 10,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 104,
  drop_type         = 10,
  drop_money        = 0,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 73,
  sell_tab0         = 73,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 200,
  hit_material_type = 4,
  face_icon         = 1,
  summon_mob_type   = 1,
  quest_type        = 18,
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