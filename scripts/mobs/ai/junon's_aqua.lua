registerNpc(401, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 650,
  r_weapon          = 260,
  l_weapon          = 167,
  level             = 0,
  hp                = 47,
  attack            = 194,
  hit               = 265,
  def               = 180,
  res               = 180,
  avoid             = 116,
  attack_spd        = 79,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 71,
  drop_type         = 733,
  drop_money        = 301,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 85,
  sell_tab0         = 85,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 19,
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