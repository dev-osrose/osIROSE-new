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

#include "worldmap.h"

// convert a monster to other
CMonster* CMap::ConverToMonster( CMonster* monster, UINT newmontype, bool heal )
{
    CNPCData* thisnpc = GServer->GetNPCDataByID( newmontype );
    if(thisnpc==NULL)// invalid type
        return NULL;
    monster->montype = newmontype;
    monster->thisnpc = thisnpc;
    monster->SetStats( );
    if(heal)
        monster->Stats->HP = monster->GetMaxHP();
    if(monster->owner==0)
    {
        monster->MonsterDrop->mobdrop = GServer->GetDropData( monster->thisnpc->dropid );
        monster->MonsterDrop->mapdrop = GServer->GetDropData( id );
    }
    BEGINPACKET( pak, 0x774 );
    ADDWORD    ( pak, monster->clientid );
    ADDWORD    ( pak, newmontype );
    GServer->SendToVisible( &pak, monster );
    monster->OnSpawn( false );
    return monster;
}

// convert a npc to other [not working]
CMonster* CMap::ConverToMonster( CNPC* npc, UINT newmontype )
{
    CMonster* monster = AddMonster( newmontype, npc->pos, 0, NULL, NULL, 0, true );
    if(monster==NULL) // invalid montype
        return NULL;
    GServer->ClearClientID( monster->clientid );
    monster->clientid = npc->clientid;
    for(UINT i=0;i<NPCList.size();i++)
    {
        if(NPCList.at(i)==npc)
            NPCList.erase( NPCList.begin()+i );
    }
    delete npc;
    BEGINPACKET( pak, 0x774 );
    ADDWORD    ( pak, npc->clientid );
    ADDWORD    ( pak, newmontype );
    GServer->SendToVisible( &pak, monster );
    return monster;
}

// Teleport player to this map and this coord.
bool CMap::TeleportPlayer( CPlayer* player, fPoint coord, bool TelePassenger )
{
    GServer->MapList.Index[player->Position->Map]->RemovePlayer( player, false );
    player->Position->Map = id;
    player->Position->current = coord;
    player->Position->destiny = coord;
    player->Session->inGame = false;
    player->Position->lastMoveTime = clock();
    if(player->Stats->HP<1)player->Stats->HP=player->Stats->MaxHP * 10 / 100;
    if(!allowpat || !TelePassenger)
    {
        if(!allowpat)
            player->Status->Stance=0x03;
        player->Ride->Drive = false;
        player->Ride->charid= 0;
        player->Ride->Ride = false;
    }
    AddPlayer( player );
	BEGINPACKET( pak, 0x07a8 );
	ADDWORD    ( pak, player->clientid );
	ADDWORD    ( pak, player->Position->Map );
	ADDFLOAT   ( pak, player->Position->current.x*100 );
	ADDFLOAT   ( pak, player->Position->current.y*100 );
    ADDWORD    ( pak, (player->Status->Stance==0x04?0x0201:0x0001) );
	player->client->SendPacket( &pak );
    if( player->Ride->Drive && player->Ride->charid!=0 )
    {
            CPlayer* otherclient = GServer->GetClientByCID( player->Ride->charid );
            if( otherclient!=NULL )
            {
                if(TelePassenger)
                {
                    TeleportPlayer( otherclient, coord );
                }
                else
                {
                    otherclient->Ride->Drive = false;
                    otherclient->Ride->charid= 0;
                    otherclient->Ride->Ride = false;
                }
            }
    }
    GServer->pakClearUser( player );
    GServer->ClearClientID( player->clientid );
    player->RestartPlayerVal( );
    return true;
}

bool CMap::pakTeleport( CPlayer* player, unsigned int Map, fPoint coord, bool TelePassenger )
{
    GServer->MapList.Index[player->Position->Map]->RemovePlayer( player, false );
    player->Position->Map = Map;
    player->Position->current = coord;
    player->Position->destiny = coord;
    player->Session->inGame = false;
    player->Position->lastMoveTime = clock();
    if(!allowpat || !TelePassenger)
    {
        if(!allowpat)
            player->Status->Stance=0x03;
        player->Ride->Drive = false;
        player->Ride->charid= 0;
        player->Ride->Ride = false;
    }
    AddPlayer( player );
	BEGINPACKET( pak, 0x07a8 );
	ADDWORD    ( pak, player->clientid );
	ADDWORD    ( pak, player->Position->Map );
	ADDFLOAT   ( pak, player->Position->current.x*100 );
	ADDFLOAT   ( pak, player->Position->current.y*100 );
    ADDWORD    ( pak, (player->Status->Stance==0x04?0x0201:0x0001) );
	player->client->SendPacket( &pak );
    if( player->Ride->Drive && player->Ride->charid!=0 )
    {
            CPlayer* otherclient = GServer->GetClientByCID( player->Ride->charid );
            if( otherclient!=NULL )
            {
                if(TelePassenger)
                {
                    TeleportPlayer( otherclient, coord );
                }
                else
                {
                    otherclient->Ride->Drive = false;
                    otherclient->Ride->charid= 0;
                    otherclient->Ride->Ride = false;
                }
            }
    }
    GServer->pakClearUser( player );
    GServer->ClearClientID( player->clientid );
    player->RestartPlayerVal( );
    return true;
}

// return near respawn [NULL if there are no respawn]
CRespawnPoint* CMap::GetNearRespawn( CPlayer* player )
{
    CRespawnPoint* respawn = NULL;
    float distance = 0xffff;
    for(UINT i=0;i<RespawnList.size();i++)
    {
        float resdist = GServer->distance( player->Position->current, RespawnList.at(i)->dest );
        if( resdist < distance )
        {
            respawn = RespawnList.at(i);
            distance = resdist;
        }
    }
    return respawn;
}

// Return "start" respawn
CRespawnPoint* CMap::GetStartRespawn( )
{
    for( UINT i = 0; i < RespawnList.size(); i++ )
    {
        if( RespawnList.at(i)->masterdest )
          return RespawnList.at(i);
    }
    return NULL;
}

// Return first respawn
CRespawnPoint* CMap::GetFirstRespawn( )
{
    CRespawnPoint* respawn = NULL;
    for(UINT i=0;i<RespawnList.size();i++)
    {
        return RespawnList.at(i);
    }
    return respawn;
}

// Search a client by clientid
CPlayer* CMap::GetPlayerInMap( UINT id )
{
    for(UINT i=0;i<PlayerList.size();i++)
    {
        if(PlayerList.at(i)->clientid == id )
            return PlayerList.at(i);
    }
    return NULL;
}

// Search a client by charid
CPlayer* CMap::GetCharIDInMap( UINT id )
{
    for(UINT i=0;i<PlayerList.size();i++)
    {
        if(PlayerList.at(i)->CharInfo->charid == id )
            return PlayerList.at(i);
    }
    return NULL;
}

// Search a monster by clientid
CMonster* CMap::GetMonsterInMap( UINT id )
{
    for(UINT i=0;i<MonsterList.size();i++)
    {
        if(MonsterList.at(i)->clientid == id )
            return MonsterList.at(i);
    }
    return NULL;
}

CDrop* CMap::GetDropInMap( UINT id )
{
    for(UINT i=0;i<DropsList.size();i++)
    {
        if(DropsList.at(i)->clientid == id )
            return DropsList.at(i);
    }
    return NULL;
}

CNPC* CMap::GetNPCInMap( UINT id )
{
    for(UINT i=0;i<NPCList.size();i++)
    {
        if(NPCList.at(i)->clientid == id )
            return NPCList.at(i);
    }
    return NULL;
}

void CMap::UpdateTime( )
{
    if(id==0) return;
    UINT etime = (UINT)GServer->round((clock() - LastUpdate)/1000);
    if(etime<10)return;
    MapTime += (UINT)floor((double)(etime/10));
    if(MapTime==0xffffffff)
        MapTime = 0;
    if(dayperiod==0)
        dayperiod=1;
    UINT ctime = MapTime%dayperiod;
    if(ctime>=morningtime && ctime<daytime)
    {
        CurrentTime = MORNING;
    }
    else
    if(ctime>=daytime && ctime<eveningtime)
    {
        CurrentTime = DAY;
    }
    else
    if(ctime>=eveningtime && ctime<nighttime)
    {
        CurrentTime = EVENING;
    }
    else
    if(ctime>=nighttime)
    {
        CurrentTime = NIGHT;
    }
    LastUpdate = clock();
}

// Clean Drops
void CMap::CleanDrops( )
{
    for(UINT j=0;j<DropsList.size();j++)
    {
        CDrop* thisdrop = DropsList.at(j);
        if( time(NULL) - thisdrop->droptime >= 50 )
            DeleteDrop( thisdrop );
    }
}

// Respawn a Monster
void CMap::RespawnMonster( )
{
#ifndef USEIFO
    for(UINT k=0;k<MonsterSpawnList.size( );k++)
    {
        CSpawnArea* thisspawn = MonsterSpawnList.at(k);

        if (thisspawn->respawntime<2)thisspawn->respawntime=2;
        clock_t rtime = clock() - thisspawn->lastRespawnTime;
        if((rtime > thisspawn->respawntime*CLOCKS_PER_SEC && thisspawn->amon<thisspawn->max)|| thisspawn->amon<thisspawn->min )
        {
            fPoint position = GServer->RandInPoly( thisspawn->points, thisspawn->pcount );
            AddMonster( thisspawn->montype, position, 0, thisspawn->mobdrop, thisspawn->mapdrop, thisspawn->id );
            thisspawn->lastRespawnTime = clock();
        }
    }
#else
    for (UINT j = 0; j < MobGroupList.size(); j++) {
      CMobGroup* thisgroup = MobGroupList.at(j);
      clock_t rtime = clock() - thisgroup->lastRespawnTime;
      if (rtime < thisgroup->respawntime*CLOCKS_PER_SEC || thisgroup->active >= thisgroup->limit)
        continue;
      for (UINT k = thisgroup->active; k < thisgroup->limit; k++) {
        CMob *thismob;
        if (thisgroup->tacMobs.size() > 0 && thisgroup->basicKills >= thisgroup->tacticalpoints) {
          thismob = thisgroup->tacMobs.at(thisgroup->curTac);
          thisgroup->basicKills = 0;
          thisgroup->curTac++;
          if (thisgroup->curTac >= thisgroup->tacMobs.size()) thisgroup->curTac = 0;
        } else {
          thismob = thisgroup->basicMobs.at(thisgroup->curBasic);
          thisgroup->curBasic++;
          if (thisgroup->curBasic >= thisgroup->basicMobs.size()) thisgroup->curBasic = 0;
        }
        for (UINT i = 0; i < thismob->amount; i++) {
        fPoint position = GServer->RandInCircle( thisgroup->point, thisgroup->range );
        AddMonster( thismob->mobId, position, 0, thismob->mobdrop, thismob->mapdrop, thisgroup->id );
        }
      }
    }
#endif
}

// Return true if is night
bool CMap::IsNight( )
{
    if(CurrentTime==NIGHT)
        return true;
    return false;
}

CCharacter* CMap::GetCharInMap( unsigned int id )
{
    CMonster* monster = GetMonsterInMap( id );
    if(monster!=NULL)
        return (CCharacter*) monster;
    CPlayer* player = GetPlayerInMap( id );
    if(player!=NULL)
        return (CCharacter*) player;
    return NULL;
}
