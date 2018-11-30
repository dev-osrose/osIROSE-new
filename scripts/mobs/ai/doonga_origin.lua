registerNpc(153, {
  walk_speed        = 0,
  run_speed         = 230,
  scale             = 700,
  r_weapon          = 170,
  l_weapon          = 0,
  level             = 0,
  hp                = 68,
  attack            = 30,
  hit               = 307,
  def               = 161,
  res               = 200,
  avoid             = 138,
  attack_spd        = 102,
  is_magic_damage   = 95,
  ai_type           = 0,
  give_exp          = 24,
  drop_type         = 56,
  drop_money        = 172,
  drop_item         = 12,
  union_number      = 12,
  need_summon_count = 45,
  sell_tab0         = 45,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 220,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 13,
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