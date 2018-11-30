registerNpc(369, {
  walk_speed        = 0,
  run_speed         = 290,
  scale             = 800,
  r_weapon          = 200,
  l_weapon          = 0,
  level             = 0,
  hp                = 108,
  attack            = 42,
  hit               = 550,
  def               = 263,
  res               = 631,
  avoid             = 192,
  attack_spd        = 173,
  is_magic_damage   = 115,
  ai_type           = 0,
  give_exp          = 144,
  drop_type         = 142,
  drop_money        = 385,
  drop_item         = 0,
  union_number      = 0,
  need_summon_count = 58,
  sell_tab0         = 58,
  sell_tab1         = 0,
  sell_tab2         = 0,
  sell_tab3         = 0,
  can_target        = 0,
  attack_range      = 0,
  npc_type          = 250,
  hit_material_type = 8,
  face_icon         = 0,
  summon_mob_type   = 0,
  quest_type        = 17,
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