registerNpc(565, {
  walk_speed        = 0,
  run_speed         = 200,
  scale             = 600,
  r_weapon          = 200,
  l_weapon          = 1113,
  level             = 0,
  hp                = 140,
  attack            = 30,
  hit               = 551,
  def               = 408,
  res               = 391,
  avoid             = 662,
  attack_spd        = 256,
  is_magic_damage   = 80,
  ai_type           = 0,
  give_exp          = 299,
  drop_type         = 113,
  drop_money        = 481,
  drop_item         = 40,
  union_number      = 40,
  need_summon_count = 20,
  sell_tab0         = 20,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 1100,
  hit_material_type = 7,
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