registerNpc(205, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 700,
  r_weapon          = 170,
  l_weapon          = 1003,
  level             = 302,
  hp                = 25,
  attack            = 74,
  hit               = 137,
  def               = 121,
  res               = 99,
  avoid             = 56,
  attack_spd        = 33,
  is_magic_damage   = 85,
  ai_type           = 0,
  give_exp          = 55,
  drop_type         = 165,
  drop_money        = 200,
  drop_item         = 20,
  union_number      = 20,
  need_summon_count = 70,
  sell_tab0         = 70,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
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