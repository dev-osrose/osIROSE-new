registerNpc(371, {
  walk_speed        = 0,
  run_speed         = 190,
  scale             = 700,
  r_weapon          = 200,
  l_weapon          = 1084,
  level             = 0,
  hp                = 102,
  attack            = 52,
  hit               = 544,
  def               = 256,
  res               = 601,
  avoid             = 232,
  attack_spd        = 130,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 129,
  drop_type         = 127,
  drop_money        = 381,
  drop_item         = 31,
  union_number      = 31,
  need_summon_count = 56,
  sell_tab0         = 56,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 800,
  hit_material_type = 9,
  face_icon         = 2,
  summon_mob_type   = 2,
  quest_type        = 17,
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