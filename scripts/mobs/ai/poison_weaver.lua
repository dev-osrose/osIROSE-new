registerNpc(526, {
  walk_speed        = 0,
  run_speed         = 240,
  scale             = 530,
  r_weapon          = 170,
  l_weapon          = 0,
  level             = 0,
  hp                = 156,
  attack            = 35,
  hit               = 752,
  def               = 441,
  res               = 473,
  avoid             = 329,
  attack_spd        = 239,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 300,
  drop_type         = 126,
  drop_money        = 448,
  drop_item         = 25,
  union_number      = 25,
  need_summon_count = 40,
  sell_tab0         = 40,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 3,
  face_icon         = 0,
  summon_mob_type   = 0,
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