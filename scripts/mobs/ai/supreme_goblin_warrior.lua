registerNpc(760, {
  walk_speed        = 0,
  run_speed         = 225,
  scale             = 850,
  r_weapon          = 180,
  l_weapon          = 169,
  level             = 0,
  hp                = 101,
  attack            = 35,
  hit               = 398,
  def               = 225,
  res               = 320,
  avoid             = 228,
  attack_spd        = 153,
  is_magic_damage   = 100,
  ai_type           = 0,
  give_exp          = 40,
  drop_type         = 30,
  drop_money        = 0,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 0,
  sell_tab0         = 0,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 18,
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