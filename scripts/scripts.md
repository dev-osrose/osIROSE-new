# Scripts
This folder contains all of the lua scripts that the server relies on to provide gameplay.
For now, it contains information about the npcs, monster spawners, user spawns and warpgates. When the [WorldServer](../src/map/map.md) starts, it loads the root.lua file that contains all of the necessary files to include.
The API is as follow:

## Spawners
`mob((string)spawner_alias, (int)mob_id, (int)spawn_max, (int)max_mobs, (int)spawn_interval, (int)spawner_range, (int)map_id, (float)x_pos, (float)y_pos, (float)z_pos);`
* `spawner_alias`: a handle for easy manipulation through the client (not yet implemented)
* `mob_id`: the id of the spawned mob
* `spawn_max`: the maximum number of mobs that can spawn at the same time from that spawn
* `max_mobs`: the maximum number of mobs that can be alive at the same time for that spawn
* `spawn_interval`: the cooldown time between each spawn
* `spawner_range`: the maximum range at which a monster can spawn
* `map_id`: the map the spawner is on
* `x_pos`: the x position
* `y_pos`: the y position
* `z_pos`: the z position
The spawner for now is really dumb: it'll generate a random number of mobs every time the cooldown is ready until it reaches the max number of mobs.
The goal is to make it dependent on triggers. For example, spawning a king if a lot of small monsters of the same familly is slained.

## Npcs
`npc((string)npc_lua_file, int)npc_id, (int)map_id, (float)x_pos, (float)y_pos, (float)z_pos, (float)angle);`
* `npc_lua_file`: right now, the npc id that should be used (that id refers to a npc present in the client's data). The goal is to have our own lua script per npc, to make them fully scriptable
* `npc_id`: the npc id (the visuals)
* `map_id`: the map the npc is on
* `x_pos`: the x position
* `y_pos`: the y position
* `z_pos`: the z position
* `angle`: the angle in degrees

## User spawns
TODO

## Warp gates
`warp_gate((string)warp_alias, (int)dest_map_id, (float)dest_x_pos, (float)dest_y_pos, (float)dest_z_pos, (int)map_id, (float)x_pos, (float)y_pos, (float)z_pos, (float)angle>, (float)x_scale, (float)y_scale, (float)z_scale);`
TODO (API may change)
