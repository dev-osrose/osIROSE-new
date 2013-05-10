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
#include "worldmonster.h"

// check is a player is in range
bool CMonster::PlayerInRange()
{
    CMap* map = GServer->MapList.Index[Position->Map];
    for(int i=0; i<map->PlayerList.size(); i++)
    {
        CPlayer* thisclient = map->PlayerList.at(i);
        if(GServer->IsVisible(thisclient, this)) return true;
    }
    return false;
}

// get the near player
CPlayer* CMonster::GetNearPlayer( UINT mdist )
{
    CPlayer* thisplayer = NULL;
    float distance = 0xffff;
    for(UINT i=0;i<GServer->MapList.Index[Position->Map]->PlayerList.size();i++)
    {
        CPlayer* thisclient = GServer->MapList.Index[Position->Map]->PlayerList.at(i);
        float tempdist = GServer->distance( Position->current, thisclient->Position->current );
        if(tempdist<distance)
        {
            thisplayer = thisclient;
            distance = tempdist;
        }
    }
    if(distance>mdist)
        return NULL; // near player is too far
    return thisplayer;
}

// return true if is a ghost
bool CMonster::IsGhost( )
{
    // candle host, elec ghost, yigore ghost
    if((montype>710 && montype<751) || (montype>679 && montype<693))
        return true;
    return false;
}

// return true if is a ghost seed
bool CMonster::IsGhostSeed( )
{
    if((montype>700 && montype<711) && montype!=703)
        return true;
    return false;
}

// return true if current monster can move
bool CMonster::CanMove( )
{
    if((montype<41 || montype>45) && (montype<325 || montype>329) && montype!=659 && (montype<771 || montype>810 ) && montype!=992 && (montype<1474 || montype>1489))
        return true;
    return false;
}

// return true if this monster is ready to move
bool CMonster::ReadyToMove( )
{
    return true;
}

// return a player pointers
CPlayer* CMonster::GetOwner( )
{
    if(!IsSummon( )) return NULL;
    CPlayer* ownerclient = GServer->GetClientByID( owner, Position->Map );
    return ownerclient;
}

bool CMonster::IsMonster( )
{
    return true;
}

void CMonster::AddDamage( CCharacter* enemy, long int hitpower)
{
    CPlayer* player = NULL;
    if(enemy->IsMonster( ))
    {
        CMonster* monster = (CMonster*) enemy;
        if(!enemy->IsSummon( )) return;
        player = monster->GetOwner( );
        if(player==NULL) return;
    }
    else
    {
        player = (CPlayer*) enemy;
    }
    if(MonsterDrop->firsthit==0 && hitpower > 0)
    {
        MonsterDrop->firsthit = player->CharInfo->charid;
        MonsterDrop->firstlevel = player->Stats->Level;
        if( player->Party->party!=NULL )
            thisparty = player->Party->party;
        else
            thisparty = NULL;
    }
    for(UINT i=0;i<PlayersDamage.size();i++)
    {
        MonsterDamage* thisplayer = PlayersDamage.at(i);
        if(thisplayer->charid == player->CharInfo->charid)
        {
            if( hitpower > Stats->HP )
                hitpower = (long int)Stats->HP + (long int)( Stats->HP * 0.10 );
            thisplayer->damage += hitpower;
            return;
        }
    }
    MonsterDamage* newplayer = new MonsterDamage;
    newplayer->charid = player->CharInfo->charid;
    if( hitpower > Stats->HP )
        hitpower = (long int)Stats->HP + (long int)( Stats->HP  * 0.10 );
    newplayer->damage = hitpower;
    PlayersDamage.push_back( newplayer );
}

CDrop* CMonster::GetDrop( )
{
    return GServer->GetPYDrop( this, 1 );
}
