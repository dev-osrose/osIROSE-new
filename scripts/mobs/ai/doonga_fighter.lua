registerNpc(154, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 700,
  r_weapon          = 130,
  l_weapon          = 1002,
  level             = 0,
  hp                = 62,
  attack            = 32,
  hit               = 291,
  def               = 140,
  res               = 156,
  avoid             = 127,
  attack_spd        = 93,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 5,
  drop_type         = 53,
  drop_money        = 173,
  drop_item         = 15,
  union_number      = 15,
  need_summon_count = 52,
  sell_tab0         = 52,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 240,
  hit_material_type = 3,
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