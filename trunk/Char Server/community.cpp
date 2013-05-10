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

// Change messenger status 07 - online | 08 - offline
bool CCharServer::ChangeMessengerStatus (CCharClient* thisclient, CCharClient* otherclient, int status)
{
        BEGINPACKET( pak, 0x7e1 );
        ADDBYTE    ( pak, 0x08 );
        ADDWORD    ( pak, thisclient->charid );
        ADDWORD    ( pak, 0x0000 );
        ADDBYTE    ( pak, status );
        otherclient->SendPacket(&pak);
        return true;
}

// Messenger actions (add/remove/invite)
bool CCharServer::pakMessengerManager ( CCharClient* thisclient, CPacket* P )
{
    BYTE action = GETBYTE((*P),0);
    switch (action)
    {
        case 0x01://wanna be my friend?
        {
            char* nick = new (nothrow) char[P->Size];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory: pakMessengerManager 1" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size );
            CCharClient* otherclient = (CCharClient*) GetClientByName (nick);
            if(otherclient!=NULL)
            {//Send friend invitation  (check this one)
                BEGINPACKET( pak, 0x7e1 );
                ADDBYTE    ( pak, 0x01 );
                ADDWORD    ( pak, 0x0000 );
                ADDSTRING  ( pak, thisclient->charname );
                ADDBYTE    ( pak, 0x00 );
                otherclient->SendPacket(&pak);
            }
            else
            {//This charname doesnt exist
               BEGINPACKET( pak, 0x7e1 );
               ADDBYTE    ( pak, 0x04 );
               ADDSTRING  ( pak, nick );
               ADDBYTE    ( pak, 0x00 );
               thisclient->SendPacket(&pak);
            }
            delete []nick;
        }
        break;
        case 0x02://yes i want
        {
            char* nick = new (nothrow) char[P->Size];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory: pakMessengerManager 2" );
                return false;
            }
            memcpy( nick, &P->Buffer[3], P->Size );
            CCharClient* otherclient = (CCharClient*) GetClientByName (nick);
            if(otherclient!=NULL)
            {
                BEGINPACKET( pak, 0x7e1 );
                ADDBYTE    ( pak, 0x02 );
                ADDWORD    ( pak, thisclient->charid );
                ADDBYTE    ( pak, 0x00 );
                ADDWORD    ( pak, 0x0000 );
                ADDSTRING  ( pak, thisclient->charname );
                ADDBYTE    ( pak, 0x00);
                otherclient->SendPacket(&pak);
                //Add friend to my friend list(sql)
                if(!DB->QExecute("INSERT INTO list_friend (id,idfriend,namefriend) VALUES (%i,%i,'%s')",
                       otherclient->charid,thisclient->charid,thisclient->charname))
                    return false;
                CFriendList * newfriend1 = new (nothrow) CFriendList;
                if(newfriend1==NULL)
                    return false;
                newfriend1->id = otherclient->charid; //friendid
                strcpy(newfriend1->name, otherclient->charname); //friend name
                thisclient->FriendList.push_back( newfriend1 );
                RESETPACKET( pak, 0x7e1 );
                ADDBYTE    ( pak, 0x02 );
                ADDWORD    ( pak, otherclient->charid );
                ADDBYTE    ( pak, 0x00 );
                ADDWORD    ( pak, 0x0000 );
                ADDSTRING  ( pak, otherclient->charname );
                ADDBYTE    ( pak, 0x00);
                thisclient->SendPacket(&pak);
                //Add me to his friend list (sql)
                if(!DB->QExecute("INSERT INTO list_friend (id,idfriend,namefriend) VALUES (%i,%i,'%s')",
                       thisclient->charid,otherclient->charid,otherclient->charname))
                    return false;
                CFriendList * newfriend2 = new (nothrow) CFriendList;
                if(newfriend2==NULL)
                    return false;
                newfriend2->id = thisclient->charid; //friendid
                strcpy(newfriend2->name, thisclient->charname); //friend name
                otherclient->FriendList.push_back( newfriend2 );
            }
            else//not founded
            {
               BEGINPACKET( pak, 0x7e1 );
               ADDBYTE    ( pak, 0x04 );
               ADDWORD    ( pak, 0x0000 );
               ADDSTRING  ( pak, nick );
               ADDBYTE    ( pak, 0x00 );
               thisclient->SendPacket(&pak);
            }
            delete []nick;
        }
        break;
        case 0x03://no, i dont want
        {
            char* nick = new (nothrow) char[P->Size];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory: pakMessengerManager 3" );
                return false;
            }
            memcpy( nick, &P->Buffer[3], P->Size );
            CCharClient* otherclient = (CCharClient*) GetClientByName (nick);
            if(otherclient!=NULL)
            {
                BEGINPACKET( pak, 0x7e1 );
                ADDBYTE    ( pak, 0x03 );
                ADDSTRING  ( pak, thisclient->charname );
                ADDBYTE    ( pak, 0x00);
                otherclient->SendPacket(&pak);
            }
            else
            {
               BEGINPACKET( pak, 0x7e1 );
               ADDBYTE    ( pak, 0x04 );
               ADDWORD    ( pak, 0x0000 );
               ADDSTRING  ( pak, nick );
               ADDBYTE    ( pak, 0x00 );
               thisclient->SendPacket(&pak);
            }
        }
        break;
        case 0x05://freakin later
        {
            WORD id = GETWORD ((*P),1);
            if(!DB->QExecute("DELETE FROM list_friend WHERE id=%i and idfriend=%i",
                       thisclient->charid,id))
                return false;
            CCharClient* otherclient = (CCharClient*) GetClientByID(id);
            if(otherclient!=NULL)
            {
                    ChangeMessengerStatus ( thisclient, otherclient, 0x08);
            }
        }
        break;
        case 0xfa://messenger logout
        {
            WORD id = GETWORD ((*P),1);
            CCharClient* ctherclient = (CCharClient*) GetClientByID(id);
            if(ctherclient==NULL)
                return true;
            ctherclient->logout = true;
            for(UINT i=0;i<ctherclient->FriendList.size();i++)
            {
                CFriendList* thisfriend = ctherclient->FriendList.at( i );
                CCharClient* otherclient = (CCharClient*) GetClientByID( thisfriend->id );
                if(otherclient!=NULL)
                {
                    ChangeMessengerStatus ( ctherclient, otherclient, 0x08);
                }
            }
           //Logout in clan 
           ClanLogout ( ctherclient );
        }
        break;
        default:
            Log( MSG_INFO, "Friend action unknown: %i", action );
        break;
    }
    return true;
}

// Messenger Chat
bool CCharServer::pakMessengerChat ( CCharClient* thisclient, CPacket* P )
{
    WORD id = GETWORD((*P),0);
    CCharClient* otherclient = (CCharClient*) GetClientByID(id);
    if(otherclient!=NULL)
    {
        BEGINPACKET( pak, 0x7e2 );  
        ADDWORD    ( pak, thisclient->charid );
        for (int i=2;i<(P->Size-6);i++)
            ADDBYTE    ( pak, GETBYTE((*P),i ));
        otherclient->SendPacket(&pak);
    }
    return true;
}
