/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://osroseon.to.md
    
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
#include "charserver.h"

// Search client by userid
CCharClient* CCharServer::GetClientByUserID( UINT userid )
{
    for(UINT i=0;i<ClientList.size( );i++)
	{		
        CCharClient* client = (CCharClient*) ClientList.at( i );
        if(client->userid==userid)
        {
            return client;
        }
	}         
    return NULL;
}

// Search the client by char id
CCharClient* CCharServer::GetClientByID( UINT charid )
{
    for(UINT i=0;i<ClientList.size( );i++)
	{		
        CCharClient* client = (CCharClient*) ClientList.at( i );
        if(client->charid==charid)
        {
            return client;
        }
	}      
    return NULL;    
}

// Search the client by char name
CCharClient* CCharServer::GetClientByName( char *name )
{
    try
    {
        for(UINT i=0;i<ClientList.size( );i++)
    	{		
            CCharClient* client = (CCharClient*) ClientList.at( i );
            if(strcmp(client->charname, name)==0)
            {
                return client;
            }
    	}
    }
    catch(...)
    {
        return NULL;
    }
    return NULL;  
}

// Search the client by char name
CCharClient* CCharServer::GetClientByUserName( char *username )
{
    for(UINT i=0;i<ClientList.size( );i++)
	{		
        CCharClient* client = (CCharClient*) ClientList.at( i );
        if(strcmp(client->username, username)==0)
        {
            return client;
        }
	}          
    return NULL;  
}

// Search the clan by clan id
CClans* CCharServer::GetClanByID( int id )
{
    for(UINT i=0;i<ClanList.size();i++)
    {
        CClans* thisclan = ClanList.at( i );
		if (thisclan->id==id) 
            return thisclan;
	}
	return NULL;
}

//     Send packet to all the clan members
bool CCharServer::SendToClanMembers( int clanid,CPacket* pak )
{
    CClans* thisclan = GetClanByID(clanid);
    if(thisclan==0)
        return false;
    for(UINT i=0;i<thisclan->ClanMembers.size();i++)
    {
        CClanMembers* thismember = thisclan->ClanMembers.at(i);
        CCharClient* otherclient = (CCharClient*) GetClientByID (thismember->id);
        if(otherclient!=0)
        {
            otherclient->SendPacketCpy(pak);
        }
    }    
    return true;
}

//     Update clan window all members 07
bool CCharServer::UpdateClanWindow( int clanid )
{
     CClans* thisclan = GetClanByID(clanid);
    if(thisclan==0)
        return false;
     for(UINT i=0;i<thisclan->ClanMembers.size();i++)
    {
        CClanMembers* thismember = thisclan->ClanMembers.at(i);
        CCharClient* otherclient = (CCharClient*) GetClientByID (thismember->id);
        if(otherclient!=0)
        {
            BEGINPACKET( pak, 0x7e0);
            ADDBYTE    ( pak, 0x33);
            ADDWORD    ( pak, clanid);
            ADDWORD    ( pak, 0x0000);
            ADDWORD    ( pak, thisclan->back );//clan background
            ADDWORD    ( pak, thisclan->logo );//clanlogo            
            ADDBYTE    ( pak, thisclan->grade);//clan grade
            ADDBYTE    ( pak, otherclient->clan_rank);
            ADDDWORD   ( pak, thisclan->cp );
            ADDDWORD   ( pak, 0x00000064 );
            ADDDWORD   ( pak, 0x00000000 );
            ADDDWORD   ( pak, 0x00000000 );
            ADDWORD    ( pak, 0x0003 );//?????????
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, 0x0000 );
            for(int i=0;i<119;i++)
                ADDBYTE ( pak, 0x00);
            ADDSTRING  ( pak, thisclan->name);//clan name
            ADDBYTE    ( pak, 0x00);
            ADDSTRING  ( pak, thisclan->slogan);//Clan slogan
            ADDBYTE    ( pak, 0x00);
            ADDSTRING  ( pak, thisclan->news);//Clan news
            ADDBYTE    ( pak, 0x00);
            otherclient->SendPacket(&pak);
         }
    }
    return true;
}

// Get channel by id
CChanels* CCharServer::GetChannelByID( unsigned int id )
{
    for(unsigned int i=0;i<ChannelList.size();i++)
    {
        CChanels* thischannel = ChannelList.at(i);
        if(thischannel->id==id)
            return thischannel;
    }
    return NULL;
}

// Returns the current time/date
unsigned long int CCharServer::GetServerTime( )
{
	// Get time/date and write it to the beginning of the row
	time_t rawtime;							// For time
	struct tm* timeinfo;					//    "
	time	  ( &rawtime );
	timeinfo  = localtime( &rawtime );	
	unsigned long int uCurTime = 0;
	uCurTime += (timeinfo->tm_sec         * 1         );
	uCurTime += (timeinfo->tm_min         * 60        );
	uCurTime += (timeinfo->tm_hour        * 3600      ); 
	uCurTime += (timeinfo->tm_yday        * 86400     );
	uCurTime += ((timeinfo->tm_year-2000) * 86400*366 );	
	return uCurTime;
}
