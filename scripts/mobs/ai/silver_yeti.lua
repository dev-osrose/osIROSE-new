registerNpc(391, {
  walk_speed        = 0,
  run_speed         = 220,
  scale             = 550,
  r_weapon          = 180,
  l_weapon          = 1008,
  level             = 0,
  hp                = 99,
  attack            = 135,
  hit               = 562,
  def               = 331,
  res               = 526,
  avoid             = 213,
  attack_spd        = 154,
  is_magic_damage   = 110,
  ai_type           = 0,
  give_exp          = 124,
  drop_type         = 420,
  drop_money        = 382,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 99,
  sell_tab0         = 99,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 10,
  face_icon         = 1,
  summon_mob_type   = 1,
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