registerNpc(553, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 450,
  r_weapon          = 100,
  l_weapon          = 1101,
  level             = 0,
  hp                = 150,
  attack            = 28,
  hit               = 714,
  def               = 443,
  res               = 386,
  avoid             = 721,
  attack_spd        = 239,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 295,
  drop_type         = 169,
  drop_money        = 472,
  drop_item         = 50,
  union_number      = 50,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 7,
  face_icon         = 1,
  summon_mob_type   = 1,
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