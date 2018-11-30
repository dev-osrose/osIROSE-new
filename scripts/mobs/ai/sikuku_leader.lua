registerNpc(537, {
  walk_speed        = 0,
  run_speed         = 400,
  scale             = 850,
  r_weapon          = 150,
  l_weapon          = 1104,
  level             = 0,
  hp                = 151,
  attack            = 44,
  hit               = 684,
  def               = 490,
  res               = 551,
  avoid             = 409,
  attack_spd        = 278,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 292,
  drop_type         = 176,
  drop_money        = 455,
  drop_item         = 30,
  union_number      = 30,
  need_summon_count = 60,
  sell_tab0         = 60,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 320,
  hit_material_type = 8,
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