registerNpc(393, {
  walk_speed        = 0,
  run_speed         = 275,
  scale             = 640,
  r_weapon          = 200,
  l_weapon          = 1092,
  level             = 0,
  hp                = 109,
  attack            = 138,
  hit               = 624,
  def               = 361,
  res               = 768,
  avoid             = 238,
  attack_spd        = 170,
  is_magic_damage   = 100,
  ai_type           = 1,
  give_exp          = 130,
  drop_type         = 440,
  drop_money        = 384,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 99,
  sell_tab0         = 99,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1500,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 0,
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