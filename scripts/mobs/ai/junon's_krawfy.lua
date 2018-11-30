registerNpc(405, {
  walk_speed        = 0,
  run_speed         = 260,
  scale             = 720,
  r_weapon          = 340,
  l_weapon          = 0,
  level             = 0,
  hp                = 80,
  attack            = 223,
  hit               = 419,
  def               = 257,
  res               = 291,
  avoid             = 171,
  attack_spd        = 123,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 74,
  drop_type         = 618,
  drop_money        = 305,
  drop_item         = 1,
  union_number      = 1,
  need_summon_count = 93,
  sell_tab0         = 93,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
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