registerNpc(332, {
  walk_speed        = 0,
  run_speed         = 250,
  scale             = 560,
  r_weapon          = 250,
  l_weapon          = 0,
  level             = 0,
  hp                = 74,
  attack            = 31,
  hit               = 335,
  def               = 172,
  res               = 221,
  avoid             = 153,
  attack_spd        = 111,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 139,
  drop_type         = 64,
  drop_money        = 356,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 47,
  sell_tab0         = 47,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 400,
  hit_material_type = 1,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 16,
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