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
#include "worldserver.h"

// Party Actions [invite/leave/kick]
bool CWorldServer::pakPartyActions( CPlayer* thisclient, CPacket* P )
{	
    unsigned int action = GETBYTE((*P),0);
    CMap* map = MapList.Index[thisclient->Position->Map];    
    switch(action)  
    {
        case 0x00://Invita a new party
        case 0x01://invite a existent party
        {
            UINT clientid = GETWORD((*P),1);
            if(thisclient->Party->party!=NULL)
            {
                if(thisclient->Party->party->Members.size()>=thisclient->Party->party->Capacity)
                {
                    SendSysMsg( thisclient, "Party is full" );
                    return true;
                }
            }
            CPlayer* otherclient = map->GetPlayerInMap( clientid );// have to be in same map
            if(otherclient==NULL)
            {
                BEGINPACKET( pak, 0x7d1 );
                ADDBYTE    ( pak, 0x00 );//No encontro el ID
                ADDWORD    ( pak, clientid );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacketCpy( &pak );
                return true;
            }
            if(otherclient->Party->party!=NULL)
            {
                BEGINPACKET( pak, 0x7d1 );
                ADDBYTE    ( pak, 0x01 );//No puede ser solicitado (ya tiene party)
                ADDWORD    ( pak, clientid );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacketCpy( &pak );                
                return true;
            }
            if(abs(otherclient->Stats->Level-thisclient->Stats->Level)>(Config.Partygap+1))
            {
                BEGINPACKET( pak, 0x7d1 );
                ADDBYTE    ( pak, 0x07 );//Level inapropiado
                ADDWORD    ( pak, clientid );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacketCpy( &pak );                     
                return true;
            }
            BEGINPACKET( pak, 0x7d0 );
            ADDBYTE    ( pak, action );
            ADDWORD    ( pak, thisclient->clientid );
            ADDBYTE    ( pak, 0x00 );
            otherclient->client->SendPacketCpy( &pak );                                  
        }
        break;
        case 0x02://Leave Party
        {
            if(thisclient->Party->party==NULL)
                return true;      
            CParty* party = thisclient->Party->party;
            BEGINPACKET( pak, 0x7d2 );
            ADDWORD    ( pak, 0xff00 );
            ADDDWORD   ( pak, thisclient->CharInfo->charid );                        
            bool pflag = false;
            if(!party->RemovePlayer( thisclient )) //if this player is not in this party
                return true;             
            if(party->Members.size()>1)
            {
                for(UINT i=0;i<party->Members.size();i++)
                {
                    CPlayer* thismember = party->Members.at(i);
                    if(!pflag)
                    {
                        ADDDWORD( pak, thismember->CharInfo->charid );
                        if(thisclient->Party->IsMaster)
                            thismember->Party->IsMaster = true;
                        pflag = true; 
                    }
                    thismember->client->SendPacketCpy( &pak );
                }
            }
            else
            {
                for(UINT i=0;i<party->Members.size();i++)
                {
                    CPlayer* thismember = party->Members.at(i);
                    BEGINPACKET( pak, 0x7d1 );
                    ADDBYTE    ( pak, 0x05 );
                    ADDWORD    ( pak, 0x0000 );
                    ADDWORD    ( pak, 0x0000 );         
                    thismember->client->SendPacket( &pak );  
                    thismember->Party->party = NULL;
                    thismember->Party->IsMaster = true;                      
                }  
                RemoveParty( party );              
                delete party;
            }      
        }
        break;
        case 0x03: // Give leader
        {
            if(thisclient->Party->party==NULL)
                return true;                  
            unsigned int charid = GETDWORD((*P),1);            
            if( !thisclient->Party->IsMaster || thisclient->CharInfo->charid == charid )
                return true;            
            CPlayer* otherclient= GetClientByCID( charid );
            if(otherclient==NULL)
                return true;
            BEGINPACKET(pak, 0x07d1)
            ADDBYTE(pak, 8);
            ADDWORD(pak, otherclient->clientid);
            ADDWORD(pak, 0);
            otherclient->Party->IsMaster = true;
            thisclient->Party->IsMaster = false;
            thisclient->Party->party->SendToMembers( &pak );
        }
        break;        
        case 0x81: //Kick from party
        {                       
            unsigned int charid = GETDWORD((*P),1); 
            if(thisclient->Party->party==NULL)
                return true;           
            if(!thisclient->Party->IsMaster || thisclient->CharInfo->charid==charid)
                return true;   
            CParty* party = thisclient->Party->party;
            CPlayer* thismember = party->GetMemberByCharID( charid );
            if(thismember==NULL)
                return true;
            
            BEGINPACKET( pak, 0x7d1 ); // Kicked from party message
            ADDBYTE    ( pak, 0x80 );
            ADDDWORD   ( pak, thismember->CharInfo->charid );
            thismember->client->SendPacketCpy( &pak );                                              
            RESETPACKET( pak, 0x7d2 );
            ADDWORD    ( pak, 0xff00 );
            ADDDWORD   ( pak, thismember->CharInfo->charid );                        
            bool pflag = false;
            if(!party->RemovePlayer( thismember )) //if this player is not in this party
                return true;             
            if(party->Members.size()>1)
            {
                for(UINT i=0;i<party->Members.size();i++)
                {
                    CPlayer* othermember = party->Members.at(i);
                    if(!pflag)
                    {
                        ADDDWORD( pak, othermember->CharInfo->charid );
                        if(thismember->Party->IsMaster)
                            othermember->Party->IsMaster = true;
                        pflag = true; 
                    }
                    othermember->client->SendPacketCpy( &pak );
                }
            }
            else
            {
                for(UINT i=0;i<party->Members.size();i++)
                {
                    CPlayer* othermember = party->Members.at(i);
                    BEGINPACKET( pak, 0x7d1 );
                    ADDBYTE    ( pak, 0x05 );
                    ADDWORD    ( pak, 0x0000 );
                    ADDWORD    ( pak, 0x0000 );         
                    othermember->client->SendPacketCpy( &pak );  
                    othermember->Party->party = NULL;
                    othermember->Party->IsMaster = true;                      
                }  
                RemoveParty( party );              
                delete party;
            }          
        }
        break;
        default:
            Log(MSG_WARNING,"Party unknown action: %i", action);        
    } 
    return true;
}

// Party Manager
bool CWorldServer::pakPartyManager( CPlayer* thisclient, CPacket* P )
{
    unsigned int action = GETBYTE((*P),0);
    switch(action)
    {
        case 0x02://Acepto
        {
            if(thisclient->Party->party!=NULL)// have party
                return true;
            unsigned int clientid = GETWORD((*P),1);
            CPlayer* otherclient = GetClientByID( clientid, thisclient->Position->Map );
            CParty* party = otherclient->Party->party;
            if(otherclient==NULL)
            {
                BEGINPACKET( pak, 0x7d1 );
                ADDBYTE    ( pak, 0x00 );//No encontro el ID
                ADDWORD    ( pak, clientid );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacketCpy( &pak );                
                return true;
            }
            if(party!=NULL)
            {
                if(party->Members.size()>=party->Capacity)
                {
                    SendSysMsg( thisclient, "Party is Full" );
                    return true;
                }            
            }
            if(abs(otherclient->Stats->Level-thisclient->Stats->Level)>(Config.Partygap+1))
            {
                BEGINPACKET( pak, 0x7d1 );
                ADDBYTE    ( pak, 0x07 );//Level inapropiado
                ADDWORD    ( pak, clientid );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacketCpy( &pak );                     
                return true;
            }            
            BEGINPACKET( pak, 0x7d1 );
            ADDBYTE    ( pak, 0x02 );//Acepto Party
            ADDWORD    ( pak, otherclient->clientid );
            ADDBYTE    ( pak, 0x00 );
            otherclient->client->SendPacketCpy( &pak );                          
            if( party==NULL )
            {   // new party
                CParty* thisparty = new CParty;
                thisparty->AddPlayer( otherclient );                
                AddParty( thisparty ); 
                otherclient->Party->IsMaster = true;
                party = thisparty;
            }         
            //Send Party Level and Party Exp
            RESETPACKET( pak, 0x7d4 ); //
            ADDBYTE    ( pak, party->PartyLevel );
            ADDDWORD   ( pak, party->Exp );
            thisclient->client->SendPacketCpy( &pak );                                             
            thisclient->Party->IsMaster = false;        
            // Send New Party Member info to other players
            RESETPACKET( pak, 0x7d2 );
            ADDBYTE    ( pak, party->Option );
            ADDBYTE    ( pak, 0x01 );
            ADDDWORD   ( pak, thisclient->CharInfo->charid );         
            ADDWORD    ( pak, thisclient->clientid );
            ADDWORD    ( pak, thisclient->Stats->MaxHP );
            ADDWORD    ( pak, thisclient->Stats->HP );
            ADDDWORD   ( pak, 0x00000000 );
            ADDDWORD   ( pak, 0x0000000f );
            ADDWORD    ( pak, 0x1388 );
            ADDSTRING  ( pak, thisclient->CharInfo->charname );
            ADDBYTE    ( pak, 0x00 );            
            party->SendToMembers( &pak );
                        
            // Send To New Party Member the members List
            RESETPACKET( pak, 0x7d2 );
            ADDBYTE    ( pak, party->Option );
            ADDBYTE    ( pak, party->Members.size() );
            for(int i=0;i<party->Members.size();i++)
            {
                CPlayer* member= party->Members.at(i);
                ADDDWORD   ( pak, member->CharInfo->charid );
                ADDWORD    ( pak, member->clientid );
                ADDWORD    ( pak, member->Stats->MaxHP );
                ADDWORD    ( pak, member->Stats->HP );
                ADDDWORD   ( pak, 0x00000000 );
                ADDDWORD   ( pak, 0x0000000f );
                ADDWORD    ( pak, 0x1388 );
                ADDSTRING  ( pak, member->CharInfo->charname );
                ADDBYTE    ( pak, 0x00 );       
            }
            thisclient->client->SendPacketCpy( &pak );                                                    
            party->AddPlayer( thisclient );              
        }
        break;
        case 0x04://No acepto
        {
            unsigned int clientid = GETWORD((*P),1);
            CPlayer* otherclient = GetClientByID( clientid, thisclient->Position->Map );
            if(otherclient==NULL)
            {
                BEGINPACKET( pak, 0x7d1 );
                ADDBYTE    ( pak, 0x00 );//No encontro el ID
                ADDWORD    ( pak, clientid );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacketCpy( &pak );                
                return true;
            }
            BEGINPACKET( pak, 0x7d1 );
            ADDBYTE    ( pak, 0x04 );//No acepto
            ADDWORD    ( pak, thisclient->clientid );
            ADDBYTE    ( pak, 0x00 );
            otherclient->client->SendPacketCpy( &pak );                 
        }
        break;
        default:
            Log(MSG_WARNING,"Party Manager unknown action: %i", action);           
    }
    return true;	
}

// Party Chat
bool CWorldServer::pakPartyChat( CPlayer* thisclient, CPacket* P )
{
    CParty* party = thisclient->Party->party;
    if(party==NULL)
        return true;        
	BEGINPACKET( pak, 0x786 );
    ADDWORD    ( pak, thisclient->clientid );
	ADDSTRING  ( pak, P->Buffer );
	ADDBYTE    ( pak, 0 );
    party->SendToMembers( &pak );
    return true;	
}

// Change Party Options 
bool CWorldServer::pakPartyOption( CPlayer* thisclient, CPacket* P )
{
    if(!thisclient->Party->IsMaster)
        return true;
    CParty* party = thisclient->Party->party;
    if(party==NULL)
        return true;
    party->Option = GETBYTE((*P),0);
    BEGINPACKET( pak, 0x7d7 );
    ADDBYTE    ( pak, party->Option );
    ADDBYTE    ( pak, 0x00 );
    party->SendToMembers( &pak );
    return true;
}

unsigned int CWorldServer::GetMaxPartyExp( unsigned int partylevel )
{
    unsigned int exp = 540;
    exp += 100 * partylevel + ( partylevel-2*10 );
    return exp;
}
