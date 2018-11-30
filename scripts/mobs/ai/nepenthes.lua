registerNpc(521, {
  walk_speed        = 0,
  run_speed         = 100,
  scale             = 120,
  r_weapon          = 170,
  l_weapon          = 0,
  level             = 0,
  hp                = 147,
  attack            = 37,
  hit               = 718,
  def               = 460,
  res               = 446,
  avoid             = 417,
  attack_spd        = 271,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 301,
  drop_type         = 119,
  drop_money        = 445,
  drop_item         = 35,
  union_number      = 35,
  need_summon_count = 35,
  sell_tab0         = 35,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 20,
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