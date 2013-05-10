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
        bool RemovePlayer( CPlayer* player, bool clearobject=true );

    CMonster* AddMonster( UINT montype, fPoint position,  UINT owner=0, CMDrops* MonsterDrop=NULL, CMDrops* MapDrop=NULL, UINT spawnid=0, bool GetDropData=false );
    bool DeleteMonster( CMonster*, bool clearobject=false, UINT i=0 );
    bool AddDrop( CDrop* drop );
    bool DeleteDrop( CDrop* drop );
    bool AddNPC( CNPC* npc );
    bool DeleteNPC( CNPC* npc );

    CMonster* ConverToMonster( CMonster* monster, UINT newmonttype, bool heal=true );
    CMonster* ConverToMonster( CNPC* npc, UINT newmonttype );

    bool TeleportPlayer( CPlayer* player, fPoint coord, bool TelePassenger=true );
    bool pakTeleport( CPlayer* player, unsigned int Map, fPoint coord, bool TelePassenger );

    CRespawnPoint* GetNearRespawn( CPlayer* player );
    CRespawnPoint* GetFirstRespawn( );
    CRespawnPoint* GetStartRespawn( );

    CCharacter* GetCharInMap( unsigned int id );
    CPlayer* GetPlayerInMap( UINT id );
    CMonster* GetMonsterInMap( UINT id );
    CPlayer* GetCharIDInMap( UINT id );
    CDrop* GetDropInMap( UINT id );
    CNPC* GetNPCInMap( UINT id );
    void UpdateTime( );
    void CleanDrops( );
    void RespawnMonster( );
    bool IsNight( );

    void LoadZon(strings zon);
    void LoadData(strings zonpath, CStrStb* stbEvent);
    void AddIfo(strings ifo, CStrStb* stbEvent);

    UINT id;           // id from map
    UINT dayperiod;    // duration of 1 day
    UINT morningtime;  // morning start time
    UINT daytime;      // day start time
    UINT eveningtime;  // evening start time
    UINT nighttime;    // nighttime start time
    UINT ghost;        // ghosttime?
    BYTE allowpvp;     // pvp allowed?
    bool dungeon;      // Map is a dungeon?
    bool allowpat;     // pat allowed?
    // Time values
    UINT MapTime;      // Past time in this map
    clock_t LastUpdate;// Last time update in the map
    UINT CurrentTime;  // 0 = morning | 1 = day  | 2 = evening | 3 = night | 4+ = invalid

    vector<CRespawnPoint*>	    RespawnList;	  // RespawnZones List
    vector<CSpawnArea*>	        MonsterSpawnList; // Monster spawn in this map
#ifdef USEIFO
    vector<CMobGroup*>          MobGroupList; // Spawn "Zones"
#endif
    vector<CMonster*>           MonsterList;      // Monster List in this map
  	vector<CDrop*>			    DropsList;		  // Droped Items
    vector<CPlayer*>            PlayerList;       // Client list in this map;
    vector<CNPC*>               NPCList;          // NPC in this map
    vector<CTeleGate*>		    TeleGateList;	  // Telegates from this map
    std::vector<SZonPoint*> PointList;

    pthread_mutex_t MonsterMutex, DropMutex;
};

#endif
