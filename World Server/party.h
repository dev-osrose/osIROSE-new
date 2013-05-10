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
#ifndef __ROSE_PARY__
#define __ROSE_PARY__
#include "worldserver.h"
#include "datatypes.h"

class CParty
{
    public:
        CParty( );
        ~CParty( );        
        
        CPlayer* GetMember( UINT id );
        CPlayer* GetMemberByCharID( UINT charid );
        void SendToMembers( CPacket* pak, CPlayer* exclient=NULL );
        void SendToVisible( CPacket* pak, CPlayer* thisclient );        
        bool AddPlayer( CPlayer* player );
        bool RemovePlayer( CPlayer* player );
        
        UINT PartyLevel;
        UINT Exp;
        UINT Option;
        UINT LastItem;
        UINT LastETC;
        UINT LastZulies;
        UINT Capacity;
        vector<CPlayer*> Members;
        CPartyExp* PartyExp;
};

#endif
