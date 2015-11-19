/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://www.osrose.net

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    depeloped with Main erose/hrose source server + some change from the original eich source
*/
#ifndef __ROSE_MAP__
#define __ROSE_MAP__
#include "worldserver.h"
#include "player.h"
#include "character.h"
#include "worldmonster.h"
#include "datatypes.h"
class CMonster;

class CMap
{
	public:
	CMap( );
	~CMap( );

	bool AddPlayer( CPlayer* player );
	bool RemovePlayer( CPlayer* player, bool clearobject = true );

	CMonster* AddMonster( uint32_t montype, fPoint position, uint32_t owner = 0, CMDrops* MonsterDrop = NULL, CMDrops* MapDrop = NULL, uint32_t spawnid = 0, bool GetDropData = false );
	bool      DeleteMonster( CMonster*, bool clearobject = false, uint32_t i = 0 );
	bool      AddDrop( CDrop* drop );
	bool      DeleteDrop( CDrop* drop );
	bool      AddNPC( CNPC* npc );
	bool      DeleteNPC( CNPC* npc );

	CMonster* ConverToMonster( CMonster* monster, uint32_t newmonttype, bool heal = true );
	CMonster* ConverToMonster( CNPC* npc, uint32_t newmonttype );

	bool TeleportPlayer( CPlayer* player, fPoint coord, bool TelePassenger = true );
	bool pakTeleport( CPlayer* player, unsigned int Map, fPoint coord, bool TelePassenger );

	CRespawnPoint* GetNearRespawn( CPlayer* player );
	CRespawnPoint* GetFirstRespawn( );
	CRespawnPoint* GetStartRespawn( );

	CCharacter* GetCharInMap( unsigned int id );
	CPlayer*    GetPlayerInMap( uint32_t id );
	CMonster*   GetMonsterInMap( uint32_t id );
	CPlayer*    GetCharIDInMap( uint32_t id );
	CDrop*      GetDropInMap( uint32_t id );
	CNPC*       GetNPCInMap( uint32_t id );
	void        UpdateTime( );
	void        CleanDrops( );
	void        RespawnMonster( );
	bool        IsNight( );

	void LoadZon( std::string zon );
	void LoadData( std::string zonpath, CStrStb* stbEvent );
	void AddIfo( std::string ifo, CStrStb* stbEvent );

	uint32_t id;          // id from map
	uint32_t dayperiod;   // duration of 1 day
	uint32_t morningtime; // morning start time
	uint32_t daytime;     // day start time
	uint32_t eveningtime; // evening start time
	uint32_t nighttime;   // nighttime start time
	uint32_t ghost;       // ghosttime?
	uint8_t allowpvp;    // pvp allowed?
	bool dungeon;     // Map is a dungeon?
	bool allowpat;    // pat allowed?
	// Time values
	uint32_t    MapTime;     // Past time in this map
	clock_t LastUpdate;  // Last time update in the map
	uint32_t    CurrentTime; // 0 = morning | 1 = day  | 2 = evening | 3 = night | 4+ = invalid

	vector< CRespawnPoint* > RespawnList;      // RespawnZones List
	vector< CSpawnArea* >    MonsterSpawnList; // Monster spawn in this map
#ifdef USEIFO
	vector< CMobGroup* > MobGroupList; // Spawn "Zones"
#endif
	vector< CMonster* >       MonsterList;  // Monster List in this map
	vector< CDrop* >          DropsList;    // Droped Items
	vector< CPlayer* >        PlayerList;   // Client list in this map;
	vector< CNPC* >           NPCList;      // NPC in this map
	vector< CTeleGate* >      TeleGateList; // Telegates from this map
	std::vector< SZonPoint* > PointList;

	std::mutex MonsterMutex, DropMutex;
};

#endif
