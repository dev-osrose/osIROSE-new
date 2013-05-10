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

#include "party.h"

CParty::CParty( )
{
    PartyLevel = 1;
    Exp = 0;
    Option =0;
    LastItem = 0;
    LastETC = 0;
    LastZulies = 0;
    Capacity = 5;
    Members.clear();    
    PartyExp = new CPartyExp;
}

CParty::~CParty( )
{
    delete PartyExp;   
}

CPlayer* CParty::GetMember( UINT id )
{
    for(UINT i=0;i<Members.size( );i++)
    {   
        CPlayer* member = Members.at(i);
        if(member->clientid==id)
            return member;
    }    
    return NULL;
}

CPlayer* CParty::GetMemberByCharID( UINT id )
{
    for(UINT i=0;i<Members.size( );i++)
    {   
        CPlayer* member = Members.at(i);
        if(member->CharInfo->charid==id)
            return member;
    }    
    return NULL;
}

void CParty::SendToMembers( CPacket* pak, CPlayer* explayer )
{
    for(UINT i=0;i<Members.size( );i++)
    {   
        CPlayer* member = Members.at(i);
        if(member==explayer)
            continue;
        member->client->SendPacketCpy( pak );
    }
}

void CParty::SendToVisible( CPacket* pak, CPlayer* thisclient )
{
    for(UINT i=0;i<Members.size( );i++)
    {
        CPlayer* member = Members.at(i);
        if(member==thisclient)
            continue;
        if(GServer->IsVisible( thisclient, member ))
            member->client->SendPacketCpy( pak );
    }
}

bool CParty::AddPlayer( CPlayer* player )
{
    player->Party->party = this;
    Members.push_back( player );
    return true;
}

bool CParty::RemovePlayer( CPlayer* player )
{
    for(UINT i=0;i<Members.size( );i++)
    {
        if(Members.at(i)==player)
        {
            Members.erase( Members.begin( )+i );
            player->Party->IsMaster = true;
            player->Party->party = NULL;
            return true;
        }
    }
    return false;
}
        
        
        
        
        
