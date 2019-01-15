registerNpc(215, {
  walk_speed        = 0,
  run_speed         = 290,
  scale             = 630,
  r_weapon          = 270,
  l_weapon          = 167,
  level             = 0,
  hp                = 55,
  attack            = 352,
  hit               = 302,
  def               = 241,
  res               = 205,
  avoid             = 143,
  attack_spd        = 89,
  is_magic_damage   = 125,
  ai_type           = 0,
  give_exp          = 256,
  drop_type         = 1705,
  drop_money        = 271,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 80,
  sell_tab0         = 80,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 350,
  hit_material_type = 10,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 15,
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