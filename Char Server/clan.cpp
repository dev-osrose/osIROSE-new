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
#include "CharServer.h"

// Send Clan information
bool CCharServer::SendClanInfo (CCharClient* thisclient)
{
    if( thisclient->clanid > 0 )
    {
        CClans *thisclan = GetClanByID(thisclient->clanid);
        if(thisclan!=NULL)
        {
            BEGINPACKET( pak, 0x7e0);
            ADDBYTE    ( pak, 0x33);//0x33 you have invited to clan
            ADDWORD    ( pak, thisclan->id);// clan id
            ADDBYTE    ( pak, 0x00);//
            ADDBYTE    ( pak, 0x00);
            ADDWORD    ( pak, thisclan->back);//Clan Background
            ADDWORD    ( pak, thisclan->logo);//Clan logo
            ADDBYTE    ( pak, thisclan->grade);//Clan grade
            ADDBYTE    ( pak, thisclient->clan_rank);// Clan rank (0 = red rokie / 6 = master)
            ADDDWORD    ( pak, thisclan->cp);//Clan Points
            ADDDWORD   ( pak, 0x00000064);
            ADDDWORD   ( pak, 0x00000000); //Clan found
            ADDDWORD   ( pak, 0x00000000);
            ADDBYTE    ( pak, 0x01);
            for(int i=34;i<156;i++)
                ADDBYTE ( pak, 0x00);
            ADDWORD    ( pak, 0x0000);
            ADDBYTE    ( pak, 0x00);
            ADDSTRING  ( pak, thisclan->name);//Clan Name            
            ADDBYTE    ( pak, 0x00);            
            ADDSTRING  ( pak, thisclan->slogan);//Clan slogan
            ADDBYTE    ( pak, 0x00);
            ADDSTRING  ( pak, thisclan->news);//Clan news
            ADDBYTE    ( pak, 0x00);
            thisclient->SendPacket(&pak);	
            
            //Put the player online in clan
            for(UINT i=0;i<thisclan->ClanMembers.size();i++)
        	{		
                CClanMembers*	ClanMember = thisclan->ClanMembers.at( i );;
                CCharClient* otherclient = GetClientByID( ClanMember->id );
                if(otherclient!=NULL)
                    ChangeClanStatus (thisclient, otherclient, thisclient->channel);//send channel here
        	}                      
        }
    }     
    return true;
}

// Change player status in clan ((channel) = online/ 0xff = offline)
bool CCharServer::ChangeClanStatus (CCharClient* thisclient, CCharClient* otherclient, int channel)
{
     if (thisclient->charid !=otherclient->charid)
     {
        BEGINPACKET( pak, 0x7e0 );
        ADDBYTE    ( pak, 0x73 );//Change player status
        ADDBYTE    ( pak, 0x00 );
        ADDBYTE    ( pak, channel );//CANAL
        ADDWORD    ( pak, 0x0000 );
        ADDWORD    ( pak, 0x0000 );
        ADDWORD    ( pak, thisclient->level );
        ADDWORD    ( pak, thisclient->job );
        ADDSTRING  ( pak, thisclient->charname );
        ADDBYTE    ( pak, 0x00 );
        otherclient->SendPacket(&pak);
     }
     return true;
}

// Clan chat
bool CCharServer::pakClanChat ( CCharClient* thisclient, CPacket* P )
{
    if(!thisclient->isLoggedIn)
        return false;
    char* tmp;
	if (P->Buffer[0]=='/') 
    {
        char* command = strtok( (char*)&(*P).Buffer[1] , " ");
        if(command==NULL)
            return true;
        if(strcmp(command, "pak")==0)//read cpacket.txt and send it (for test packets)
        {
            int comando = 0;
            if ((tmp = strtok(NULL, " "))==NULL)
                return true;
            comando=atoi(tmp);     
            char buffer;                              
            FILE *packet1 = fopen("cpacket.txt","r");                 
            BEGINPACKET(pak,comando);               
            if(packet1==NULL)
            {
                printf("Error opening packet1.txt for 716!\n");
                return true;
            }
            while((fscanf(packet1,"%c",&buffer))!=EOF)
                    ADDBYTE(pak,buffer);
            fclose(packet1);
    		thisclient->SendPacket( &pak );            
    		return true;
        }
    }    
    char* text = new (nothrow) char[P->Size];
    if(text==NULL)
    {
        Log(MSG_ERROR, "Error allocing memory" );
        return false;
    }
    memcpy( text, &P->Buffer, P->Size );
    CClans* thisclan = (CClans*) GetClanByID ( thisclient->clanid );
    if(thisclan!=NULL)
    {
            BEGINPACKET( pak, 0x787 );
            ADDSTRING  ( pak, thisclient->charname );
            ADDBYTE    ( pak, 0x00 );
            ADDSTRING  ( pak, text );
            ADDBYTE    ( pak, 0x00 );
            SendToClanMembers(thisclan->id,&pak);
    }
    delete []text;
    return true;
}

// Clan manager for all the clan functions
bool CCharServer::pakClanManager ( CCharClient* thisclient, CPacket* P )
{    
    if(!thisclient->isLoggedIn) return false;
    MYSQL_RES *result;
	MYSQL_ROW row;
    BYTE action = GETBYTE ((*P),0);
    switch (action)
    {
        case 0x01:
        break;
        case 0x02://invite clan
        {
            char* nick = new (nothrow) char[P->Size];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size );
            if(strcmp(nick,thisclient->charname)==0)
                return true;                 
            CCharClient* otherclient = (CCharClient*) GetClientByName( nick );
            if(otherclient!=NULL)
            {
                BEGINPACKET( pak, 0x7e0 );
                ADDBYTE    ( pak, 0x0b );//invite action
                ADDSTRING  ( pak, thisclient->charname );
                ADDBYTE    ( pak, 0x0 );
                otherclient->SendPacket( &pak );
            }
            delete []nick;
        }
        break;
        case 0x03://Kick from clan
        {
            char* nick = new (nothrow) char[P->Size];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size );                  
           CClans* thisclan = GetClanByID(thisclient->clanid);     
           if(thisclan==NULL)
           {
                delete []nick;
                return true;                
            }
            BEGINPACKET( pak, 0x7e0 );
	        ADDBYTE    ( pak, 0x81 );//xxx have kicket to yyyy
	        ADDSTRING  ( pak, nick );
	        ADDBYTE    ( pak, 0x00);
	        ADDSTRING  ( pak, thisclient->charname );
	        ADDBYTE    ( pak, 0x00);
            SendToClanMembers(thisclient->clanid,&pak); 
            for(UINT i=0;i<thisclan->ClanMembers.size( );i++)
            {
                CClanMembers* ClanMember =  thisclan->ClanMembers.at( i );
                if(strcmp(ClanMember->name,nick)==0)
                {
                    thisclan->ClanMembers.erase( thisclan->ClanMembers.begin() + i );
                    break;              
               }   
            }   
                                     
           CCharClient* otherclient = (CCharClient*) GetClientByName( nick ); 
           if( otherclient!=NULL )                                              
           {
               otherclient->clanid = 0;
               otherclient->clan_rank=1;                     
	           RESETPACKET( pak, 0x7e1 );//Update world clan information
	           ADDBYTE    ( pak, 0xfb );//action (kick)
	           ADDSTRING  ( pak, nick ); 
	           ADDBYTE    ( pak, 0x00 );
 	           CChanels* thischannel = GetChannelByID( otherclient->channel );
 	           if(thischannel!=NULL)
 	           {
        	       send( thischannel->sock, (char*)&pak, pak.Size, 0 );    
               }
               else { Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);}
               pakClanMembers( otherclient );            
            }           
            if(!DB->QExecute("UPDATE characters SET clanid=0 AND clan_rank=1 WHERE char_name='%s'",nick))
            {
                delete []nick;
                return false;            
            }
            delete []nick;
        }  
        break;
        case 0x04://up rank
        {
            bool online = true;
            char* nick = new (nothrow) char[P->Size-7];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size-7 );
            int clan_rank=0;
            int level=0;
            int job=0;
            int channel=0xff;
            if(strcmp(nick,thisclient->charname)==0)
            {
                delete []nick;
                return true;
            }
            CCharClient* otherclient = GetClientByName ( nick );
            if(otherclient!=NULL)
            {
                clan_rank = otherclient->clan_rank;
                if(otherclient->clan_rank<5)
                {
                    otherclient->clan_rank++;
                }
                else
                {
                    delete []nick;
                    return true;
                }
                CClans* thisclan = GetClanByID(otherclient->clanid);
                 for(UINT i=0;i<thisclan->ClanMembers.size();i++)
                {
                    CClanMembers* thismember = thisclan->ClanMembers.at(i);
                    if(thismember!=0)
                    {
                       if (thismember->id==otherclient->charid)
                       {
                           thismember->clan_rank=otherclient->clan_rank;
                       }
                    }
                }
                job = otherclient->job;
                level = otherclient->level;
                channel=otherclient->channel;
	            BEGINPACKET( pak, 0x7e0 );
	            ADDBYTE    ( pak, 0x83 );
	            ADDSTRING  ( pak, nick );
	            ADDBYTE    ( pak, 0x00 );
	            ADDBYTE    ( pak, otherclient->clan_rank );
                otherclient->SendPacket(&pak);
            }
            else
            {
                online=false;
                result = DB->QStore("SELECT clan_rank,classid,level FROM characters WHERE char_name='%s'",nick);
                if(result==NULL)
                {
                    delete []nick;
                    DB->QFree( );
                    return false;
                }
                if(mysql_num_rows(result)!=1)
                {
                    Log(MSG_WARNING, "Invalid charname: %s" , nick );
                    delete []nick;
                    DB->QFree( );
                    return false;
                }
	            row = mysql_fetch_row( result );
                clan_rank=atoi(row[0]);
                job = atoi(row[1]);
                level = atoi(row[2]);
                DB->QFree( );
            }
            if(clan_rank<5)
            {
                clan_rank++;
            }
            if(!DB->QExecute("UPDATE characters SET clan_rank=%i WHERE char_name='%s'",clan_rank,nick))
            {
                delete []nick;
                return false;
            }
            BEGINPACKET( pak, 0x7e0 );
            ADDBYTE    ( pak, 0x75 );
            ADDBYTE    ( pak, clan_rank );
            ADDBYTE    ( pak, channel );//channel
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, level ); // level
            ADDWORD    ( pak, job ); // job
            ADDSTRING  ( pak, nick ); // Nick
            ADDBYTE    ( pak, 0x00 );
            SendToClanMembers(thisclient->clanid,&pak);
             if (online==true)
	        {
    	        RESETPACKET( pak, 0x7e1 );//update world information
    	        ADDBYTE    ( pak, 0xfc );
    	        ADDBYTE    ( pak, clan_rank );
    	        ADDSTRING  ( pak, nick );
    	        ADDBYTE    ( pak, 0x00 );
     	        CChanels* thischannel = GetChannelByID( channel );
     	        if(thischannel!=NULL){
            	    send( thischannel->sock, (char*)&pak, pak.Size, 0 );}
           	    else { Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);}
            }
        	delete []nick;
        }
        break;
         case 0x05://down rank
        {
            bool online = true;
            char* nick = new (nothrow) char[P->Size-7];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size-7 );
            int clan_rank=0;
            int level=0;
            int job=0;
            int channel=0xff;
            if(strcmp(nick,thisclient->charname)==0)
            {
                delete []nick;
                return true;
            }
            CCharClient* otherclient = GetClientByName ( nick );
            if(otherclient!=NULL)
            {
                clan_rank = otherclient->clan_rank;
                if(otherclient->clan_rank>1)
                {
                    otherclient->clan_rank--;
                }
                else
                {
                    delete []nick;
                    return true;
                }
                CClans* thisclan = GetClanByID(otherclient->clanid);
                 for(UINT i=0;i<thisclan->ClanMembers.size();i++)
                {
                    CClanMembers* thismember = thisclan->ClanMembers.at(i);
                    if(thismember!=0)
                    {
                       if (thismember->id==otherclient->charid)
                       {
                           thismember->clan_rank=otherclient->clan_rank;
                       }
                    }
                }
                job = otherclient->job;
                level = otherclient->level;
                channel=otherclient->channel;
	            BEGINPACKET( pak, 0x7e0 );
	            ADDBYTE    ( pak, 0x83 );
	            ADDSTRING  ( pak, nick );
	            ADDBYTE    ( pak, 0x00 );
	            ADDBYTE    ( pak, otherclient->clan_rank );
                otherclient->SendPacket(&pak);
            }
            else
            {
                online=false;
                result = DB->QStore("SELECT clan_rank,classid,level FROM characters WHERE char_name='%s'",nick);
                if(result==NULL)
                {
                    delete []nick;
                    DB->QFree( );
                    return false;
                }
                if(mysql_num_rows(result)!=1)
                {
                    Log(MSG_WARNING, "Invalid charname: %s" , nick );
                    delete []nick;
                    DB->QFree( );
                    return false;
                }
	            row = mysql_fetch_row( result );
                clan_rank=atoi(row[0]);
                job = atoi(row[1]);
                level = atoi(row[2]);
                DB->QFree( );
            }
            if(clan_rank>1)
            {
                clan_rank--;
            }
            if(!DB->QExecute("UPDATE characters SET clan_rank=%i WHERE char_name='%s'",clan_rank,nick))
            {
                delete []nick;
                return false;
            }
            BEGINPACKET( pak, 0x7e0 );
            ADDBYTE    ( pak, 0x75 );
            ADDBYTE    ( pak, clan_rank );
            ADDBYTE    ( pak, channel );//channel
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, level ); // level
            ADDWORD    ( pak, job ); // job
            ADDSTRING  ( pak, nick ); // Nick
            ADDBYTE    ( pak, 0x00 );
            SendToClanMembers(thisclient->clanid,&pak);
             if (online==true)
	        {
    	        RESETPACKET( pak, 0x7e1 );//update world information
    	        ADDBYTE    ( pak, 0xfc );
    	        ADDBYTE    ( pak, clan_rank );
    	        ADDSTRING  ( pak, nick );
    	        ADDBYTE    ( pak, 0x00 );
     	        CChanels* thischannel = GetChannelByID( channel );
     	        if(thischannel!=NULL){
            	    send( thischannel->sock, (char*)&pak, pak.Size, 0 );}
           	    else { Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);}
            }
        	delete []nick;
        }
        break;
        case 0x06://Change news
        {
            if(thisclient->clan_rank<5)
                return true;
            char* news = new (nothrow) char[P->Size-7];
            if(news==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( news, &P->Buffer[1], P->Size-7 );
            CClans* thisclan = GetClanByID(thisclient->clanid);
            if(thisclan!=NULL)
            {
                strcpy(thisclan->news,news);
                BEGINPACKET( pak, 0x7e0 );
                ADDBYTE    ( pak, 0x34 );
                ADDSTRING  ( pak, thisclan->news );
                ADDBYTE    ( pak, 0x00 );
                SendToClanMembers(thisclan->id,&pak);             
                if(!DB->QExecute("UPDATE list_clan SET news='%s' WHERE id=%i", thisclan->news, thisclan->id))
                     return false;                            
            }
            delete []news;
        }
        break;
        case 0x07://Leave Clan
        {                 
           if( thisclient->clan_rank == 6 )
                return true;
           CClans* thisclan = GetClanByID( thisclient->clanid );
           if(thisclan==NULL)
                return true;
           BEGINPACKET( pak, 0x7e0 );
	       ADDBYTE( pak, 0x82 );
	       ADDSTRING( pak, thisclient->charname );
	       ADDBYTE( pak, 0x00);
           SendToClanMembers(thisclient->clanid,&pak);

            for(UINT i=0;i<thisclan->ClanMembers.size();i++)
            {
                CClanMembers* ClanMember = thisclan->ClanMembers.at( i );
                if(strcmp(ClanMember->name,thisclient->charname)==0)
                {
                    thisclan->ClanMembers.erase( thisclan->ClanMembers.begin()+i );
                    delete ClanMember;
                    break;              
               }   
            }                                                
            thisclient->clanid = 0;
            thisclient->clan_rank=1;                                
            if(!DB->QExecute("UPDATE characters SET clanid=0 , clan_rank=0 WHERE char_name='%s'",thisclient->charname))
                return false;
	        RESETPACKET( pak, 0x7e1 );//Update world clan information
	        ADDBYTE    ( pak, 0xfb );//action (leave)
	        ADDSTRING  ( pak, thisclient->charname ); 
	        ADDBYTE    ( pak, 0x00 );
 	        CChanels* thischannel = GetChannelByID( thisclient->channel );
 	        if(thischannel!=NULL){
        	    send( thischannel->sock, (char*)&pak, pak.Size, 0 );}
     	    else { Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);}
        }
        break;
        case 0x08:
        {
            pakClanMembers ( thisclient );                       
        }
        break;
        case 0x09://Give Master
        {           
            int clan_rank=0;
            int level=0;
            int job=0;
            int channel=0xff;            
            char* nick = new (nothrow) char[P->Size-7];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size-7 );                     
            if(strcmp(nick,thisclient->charname)==0)
            {
                delete []nick;
                return true;            
            }                
            //Convert to master
            CCharClient* otherclient = GetClientByName ( nick );
            if(otherclient!=NULL)
            {
                clan_rank = 6;                
                otherclient->clan_rank = clan_rank;                                 
                job = otherclient->job;
                level = otherclient->level;                   
                channel = otherclient->channel;
	            BEGINPACKET( pak, 0x7e0 );
	            ADDBYTE    ( pak, 0x83 );
	            ADDSTRING  ( pak, nick ); 
	            ADDBYTE    ( pak, 0x00 );	           
	            ADDBYTE    ( pak, otherclient->clan_rank );
                otherclient->SendPacket(&pak);                                
            }
            else
            {                   //   0     1
                result = DB->QStore("SELECT classid,level FROM characters where char_name='%s'",nick);
                if(result==NULL)
                {
                    delete []nick;
                    return false;                
                }
                if(mysql_num_rows(result)!=1)
                {
                    DB->QFree( );
                    Log(MSG_WARNING, "Invalid charname: %s" , nick );
                    delete []nick;
                    return false;
                }                    
	            row = mysql_fetch_row( result );		            
                job = atoi(row[0]);
                level = atoi(row[1]);
                DB->QFree( );                                 
            }            
            if(!DB->QExecute("UPDATE characters SET clan_rank=%i WHERE char_name='%s'",clan_rank,nick))
            {
                delete []nick;
                return false;
            }
            BEGINPACKET( pak, 0x7e0 );
            ADDBYTE    ( pak, 0x75 );
            ADDBYTE    ( pak, clan_rank );
            ADDBYTE    ( pak, channel );//canal   
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, level ); //level?
            ADDWORD    ( pak, job ); // job?                
            ADDSTRING  ( pak, nick );
            ADDBYTE    ( pak, 0x00 );
            SendToClanMembers(thisclient->clanid,&pak);
            
	        RESETPACKET( pak, 0x7e1 );//update world information
	        ADDBYTE    ( pak, 0xfc );
	        ADDBYTE    ( pak, clan_rank );
	        ADDSTRING  ( pak, nick ); 
	        ADDBYTE    ( pak, 0x00 );
 	        CChanels* thischannel = GetChannelByID( channel );
 	        if(thischannel!=NULL){
        	    send( thischannel->sock, (char*)&pak, pak.Size, 0 );}
     	    else { Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);}

            //Convert to commander	        
            clan_rank = 4;        
            thisclient->clan_rank=clan_rank;                                         
            job = thisclient->job;
            level = thisclient->level;                   
            channel = thisclient->channel;
	        RESETPACKET( pak, 0x7e0 );
	        ADDBYTE    ( pak, 0x83 );
	        ADDSTRING  ( pak, thisclient->charname ); 
	        ADDBYTE    ( pak, 0x00 );	           
	        ADDBYTE    ( pak, thisclient->clan_rank );
            thisclient->SendPacket(&pak);
            
            if(!DB->QExecute("UPDATE characters SET clan_rank=%i WHERE char_name='%s'",clan_rank,thisclient->charname))
            {
                delete []nick;
                return false;
            }
            RESETPACKET( pak, 0x7e0 );
            ADDBYTE    ( pak, 0x75 );
            ADDBYTE    ( pak, clan_rank );
            ADDBYTE    ( pak, channel );//canal   
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, 0x0000 );
            ADDWORD    ( pak, level ); //level?
            ADDWORD    ( pak, job ); // job?                
            ADDSTRING  ( pak, thisclient->charname );
            ADDBYTE    ( pak, 0x00 );
            SendToClanMembers(thisclient->clanid,&pak);
            
	        RESETPACKET( pak, 0x7e1 );//update world information
	        ADDBYTE    ( pak, 0xfc );
	        ADDBYTE    ( pak, clan_rank );
	        ADDSTRING  ( pak, thisclient->charname ); 
	        ADDBYTE    ( pak, 0x00 );
 	        thischannel = GetChannelByID( channel );
 	        if(thischannel!=NULL){
        	    send( thischannel->sock, (char*)&pak, pak.Size, 0 );}
     	    else { Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);}
            delete []nick;     	        
        }
        break;
        case 0x0a://disorganize clan
        {
            if(thisclient->clan_rank<6)
                return true;
            unsigned int clanid = thisclient->clanid;
            CClans* thisclan = GetClanByID(thisclient->clanid);  
            if(thisclan==NULL)
                return true;    
            if(thisclan->ClanMembers.size()>1)//Can't be deleted if there are members
                return true;            
            if(!DB->QExecute("UPDATE characters SET clanid=0 AND clan_rank=1 WHERE clanid=%i",thisclient->clanid))
                return false;
            if(!DB->QExecute("DELETE FROM list_clan WHERE id=%i",thisclient->clanid))
                return false;
            BEGINPACKET( pak, 0x7e0 );
            ADDBYTE    ( pak, 0x51 );
            ADDBYTE    ( pak, 0x00 );
            SendToClanMembers(thisclient->clanid, &pak);              
            thisclan->ClanMembers.clear( );                                         
            //delete from clan list
            for(UINT i=0;i<ClanList.size( );i++)
            {
                CClans* clan = ClanList.at( i );
                if(thisclan == clan)
                {                    
                    ClanList.erase( ClanList.begin() + i );
                    delete clan;
                    break;
                }
            }
	        RESETPACKET( pak, 0x7e1 );//Update world clan information
	        ADDBYTE    ( pak, 0xfd );//action (disorg)
	        ADDWORD    ( pak, clanid ); 
	        ADDWORD    ( pak, thisclient->charid );
 	        CChanels* thischannel = GetChannelByID( thisclient->channel );
 	        if(thischannel!=NULL){
        	    send( thischannel->sock, (char*)&pak, pak.Size, 0 );}
     	    else { Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);}
            thisclient->clanid = 0;
            thisclient->clan_rank = 1; 	        
 	        //send( wct
        }
        break;
        case 0x0c://invitation accepted
        {
            char* nick = new (nothrow) char[P->Size-7];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size-7 );                  
            CCharClient* otherclient = (CCharClient*) GetClientByName(nick );            
            if(otherclient!=NULL)
            {
                thisclient->clanid = otherclient->clanid;
                thisclient->clan_rank=1;
                if(!DB->QExecute("UPDATE characters SET clanid=%i , clan_rank=1 WHERE char_name='%s'",thisclient->clanid,thisclient->charname))
                {
                    delete []nick;
                    return false;
                }
                CClans* thisclan = GetClanByID(thisclient->clanid);
                if(thisclan==NULL)
                {
                    Log(MSG_ERROR, "Clan with id %d doesn't exist", thisclient->clanid);
                    delete []nick;
                    return true;                
                }
                CClanMembers* newmember = new (nothrow) CClanMembers;
                if(newmember==NULL)
                {
                    Log( MSG_WARNING, "Error allocing memory" ); 
                    delete []nick;                   
                    return false;
                }
                newmember->id = thisclient->charid;
                strcpy(newmember->name,thisclient->charname);  
                newmember->clan_rank = 1;                         
                thisclan->ClanMembers.push_back( newmember );
           }
           else
           {
             Log(MSG_ERROR, "Char with name %s doesn't exist", nick);
                delete []nick;
                return true;
           }    
           SendClanInfo(thisclient);
           pakClanMembers(thisclient);
           BEGINPACKET( pak, 0x7e0 );//xxx have invited to xxx
	       ADDBYTE( pak, 0x61 );
	       ADDSTRING( pak, thisclient->charname );
	       ADDBYTE( pak, 0x00);
	       ADDSTRING( pak, otherclient->charname );
	       ADDBYTE( pak, 0x00);
           SendToClanMembers(thisclient->clanid,&pak);    
           
           RESETPACKET( pak, 0x7e0 );
           ADDBYTE    ( pak, 0x84 );
           ADDWORD    ( pak, thisclient->level );
           ADDWORD    ( pak, thisclient->job );
           ADDSTRING  ( pak, thisclient->charname );
           ADDBYTE    ( pak, 0x00 );
           SendToClanMembers(thisclient->clanid,&pak);           
                                                        
	       RESETPACKET( pak, 0x7e1 );//update clan info in world
	       ADDBYTE( pak, 0xfa );
	       ADDWORD( pak, thisclient->charid );
	       ADDWORD( pak, thisclient->clanid );
 	       CChanels* thischannel = GetChannelByID( thisclient->channel );
 	       if(thischannel!=NULL) {
           send( thischannel->sock, (char*)&pak, pak.Size, 0 );
         }
    	   else {
    	     Log(MSG_ERROR, "Check serverid in worldserver.conf it should be the same as ID in channels in the mysql database %i",thisclient->channel);
         }
 	       CChanels* otherchannel = GetChannelByID( otherclient->channel );
 	       if(otherchannel!=NULL && thischannel!=otherchannel) {
                send( thischannel->sock, (char*)&pak, pak.Size, 0 );
         }
           delete []nick;	            	    
        }                    
        break;
        case 0x0d://invitation no accepted
        {
            char* nick = new (nothrow) char[P->Size-7];
            if(nick==NULL)
            {
                Log(MSG_ERROR, "Error allocing memory" );
                return false;
            }
            memcpy( nick, &P->Buffer[1], P->Size-7 );                 
            CCharClient* otherclient = (CCharClient*) GetClientByName(nick );
            if(otherclient!=NULL)
            {
                BEGINPACKET( pak, 0x7e0 );
                ADDBYTE    ( pak, 0x0d );
                ADDSTRING  ( pak, thisclient->charname );
                ADDBYTE    ( pak, 0x00 );
                otherclient->SendPacket(&pak);
            }             
            delete []nick;           
        }            
        break;
        case 0x0f:// Update Clan window with Level and Job
        {
             CClans* thisclan = (CClans*) GetClanByID( thisclient->clanid );
	         if(thisclan!=NULL)
	         {
                BEGINPACKET( pak, 0x7e0 );
                ADDBYTE    ( pak, 0x84 );
                ADDWORD    ( pak, GETWORD ((*P),1) );
                ADDWORD    ( pak, GETWORD ((*P),3) );
                ADDSTRING  ( pak, thisclient->charname );
                ADDBYTE    ( pak, 0x00 );
                SendToClanMembers(thisclient->clanid,&pak);
             }
        }
        break;
         case 0x5C://message from worldserver to load the new clan information
        {
           UINT clanid = GETWORD((*P),1);
           UINT clientrank = GETWORD((*P),3);
           
           CClans* thisclan = (CClans*) GetClanByID( clanid );
           if(thisclan==NULL)
               return true;
           thisclan->cp=GETDWORD ((*P),5);
           
            BEGINPACKET( pak, 0x7e0);
            ADDBYTE    ( pak, 0x71);
            ADDWORD    ( pak, clanid);
            ADDWORD    ( pak, 0x0000);//
            ADDWORD    ( pak, thisclan->back );//clan background
            ADDWORD    ( pak, thisclan->logo );//clanlogo            
            ADDBYTE    ( pak, thisclan->grade);//clan grade
            ADDBYTE    ( pak, 0xCC);
            ADDDWORD   ( pak, thisclan->cp );
            ADDDWORD   ( pak, 0x00000064 );
            ADDDWORD   ( pak, 0x00000000 );
            ADDDWORD   ( pak, 0x00000000 );
            ADDWORD    ( pak, 0xCCCC );
            ADDWORD    ( pak, 0xCCCC );
            ADDWORD    ( pak, 0xCCCC );
            for(int i=0;i<119;i++)
            ADDBYTE ( pak, 0x00);
            SendToClanMembers(clanid,&pak);
            UpdateClanWindow(clanid);
        }
        break;      
        case 0xfa://message from worldserver to load the new clan information
        {
           int id = GETWORD((*P),1); // clanid
           int charid = GETWORD((*P),3);  // charid
           int clientid = GETWORD((*P),5); // clientid
           CCharClient* otherclient = (CCharClient*) GetClientByID ( charid );
           if(otherclient==NULL)
               return true;                        
        	// Load all our clan information
        	            //     0    1  2    3    4   5   6       7
        	result = DB->QStore("SELECT id,logo,back,name,cp,grade,slogan,news FROM list_clan where id=%i",id);
            if(result == NULL) return false;        	
            if(mysql_num_rows(result)!=1)
            {
                DB->QFree( );
                Log(MSG_WARNING, "Invalid clan: %i" , id );
                return false;
            }             	
        	row = mysql_fetch_row(result);
        	//clan info
        	CClans* newclan = new CClans;
            assert(newclan);          
        	newclan->id = atoi(row[0]);
        	newclan->logo = atoi(row[1]);
        	newclan->back = atoi(row[2]);
        	strncpy(newclan->name,row[3],16);
        	newclan->cp = atoi(row[4]);
        	newclan->grade = atoi(row[5]);	
            strncpy(newclan->slogan,row[6],29);	
            strcpy(newclan->news,row[7]);
            ClanList.push_back( newclan );	
            DB->QFree( );
            //member info
            CClanMembers* newmember = new CClanMembers;
            newmember->id = otherclient->charid;
            strcpy(newmember->name,otherclient->charname);
            newmember->clan_rank = 6;
            newclan->ClanMembers.push_back( newmember );            	
        				
            otherclient->clanid = id;
            otherclient->clan_rank = 6;
            //Send new clan information    
            BEGINPACKET( pak, 0x7e0);
            ADDBYTE    ( pak, 0x30);
            ADDWORD    ( pak, clientid);
            ADDWORD    ( pak, 0x0100);//
            ADDWORD    ( pak, newclan->back );//clan background
            ADDWORD    ( pak, newclan->logo );//clanlogo            
            ADDBYTE    ( pak, newclan->grade);//clan grade
            ADDBYTE    ( pak, otherclient->clan_rank);// clan rank
			ADDDWORD   ( pak, newclan->cp );
            ADDDWORD   ( pak, 0x00000000 ); // Unk
            ADDDWORD   ( pak, 0x00000000 ); // Clan Funds
            ADDDWORD   ( pak, 0x00000000 ); // Unk
            ADDWORD    ( pak, 0x0000 ); // Unk
            ADDDWORD   ( pak, 0x00000000 ); // Contribution
            for(int i=0;i<120;i++)
                ADDBYTE ( pak, 0x00);
            ADDSTRING  ( pak, newclan->name);//clan name
            ADDBYTE    ( pak, 0x00);
            ADDSTRING  ( pak, newclan->slogan);//clan slogan
            ADDBYTE    ( pak, 0x00);

            otherclient->SendPacket(&pak);                      
        }
        break;
        case 0xfb:
        {
             UINT clanid = GETWORD((*P),1);
             UINT grade = GETWORD((*P),3);
             CClans* thisclan = (CClans*) GetClanByID( clanid );
	         if(thisclan!=NULL)
	         {
                BEGINPACKET( pak, 0x7e0 );
                ADDBYTE    ( pak, 0x84 );
                ADDWORD    ( pak, GETWORD ((*P),1) );
                ADDWORD    ( pak, GETWORD ((*P),3) );
                ADDSTRING  ( pak, thisclient->charname );
                ADDBYTE    ( pak, 0x00 );
                SendToClanMembers(thisclient->clanid,&pak);
             }
        }         
        default:
            Log( MSG_INFO, "Clan action unknow %i ", action);
        break;
    }
    return true;
}

// Send clan members
bool CCharServer::pakClanMembers ( CCharClient* thisclient )
{    
	CClans* thisclan = (CClans*) GetClanByID( thisclient->clanid );
	if(thisclan!=NULL)
	{
        BEGINPACKET( pak, 0x7e0 );
        ADDBYTE    ( pak, 0x72 );//Send clan members
        for(UINT i=0;i<thisclan->ClanMembers.size( );i++)
    	{
            CClanMembers* thismember = thisclan->ClanMembers.at( i );
            if(thismember==NULL)
                continue;                                                                 
            CCharClient* otherclient = (CCharClient*) GetClientByID (thismember->id);
            if(otherclient!=NULL)
            {
                ADDBYTE    ( pak, thismember->clan_rank ); //clan rank
                ADDBYTE    ( pak, otherclient->channel ); //channel (0x01 = channel 1)
                ADDWORD    ( pak, 0x0000 );       
                ADDWORD    ( pak, 0x0000 );                   
                ADDWORD    ( pak, otherclient->level );// Level
                ADDWORD    ( pak, otherclient->job );// Job
            }
            else
            {          
                ADDBYTE    ( pak, thismember->clan_rank); //clan rank
                ADDBYTE    ( pak, 0xff ); //channel (0xff = offline)
                ADDWORD    ( pak, 0x0000 );       
                ADDWORD    ( pak, 0x0000 );                   
                ADDWORD    ( pak, 0x0000 );// Level     
                ADDWORD    ( pak, 0x0000 );// Job                 
            }                
            ADDSTRING  ( pak, thismember->name );
            ADDBYTE    ( pak, 0x00 );  
        }        
        thisclient->SendPacketCpy( &pak );            
    }
    return true;
}

// Logout from clan
bool CCharServer::ClanLogout ( CCharClient* thisclient )
{    
    CClans* thisclan = (CClans*) GetClanByID(thisclient->clanid);             
    if(thisclan!=NULL)
    {       
        for(UINT i=0;i<thisclan->ClanMembers.size();i++)
        {
            CClanMembers* thismember = thisclan->ClanMembers.at( i );
            CCharClient* otherclient = (CCharClient*) GetClientByID ( thismember->id );
            if(otherclient!=NULL)
            {
                ChangeClanStatus (thisclient, otherclient, 0xff);//send channel here
            }
        }        	
    }     
    return true;
}

// upload clanmark
bool CCharServer::pakUploadCM ( CCharClient* thisclient, CPacket* P )
{
    CClans* thisclan = GetClanByID( thisclient->clanid );
    if(thisclan==NULL) return true;    
    FILE* fh = fopen( "clanmark/clanmark.cnt", "r+b" );
    if(fh==NULL)
    {
        Log( MSG_WARNING, "Error opening clanmark counter file" );
        return true;
    }
    rewind(fh);
    unsigned int cmid = 0;// this will be our clanmark id
    unsigned int tcmid = 0;// this will be to update the id
    fread( &cmid, 1, 4, fh );  
    tcmid = cmid+1;
    rewind(fh);    
    fwrite( &tcmid, 1, 4, fh );
    fclose(fh);   
    thisclan->logo = GETWORD((*P), 0 );//cmid;
    thisclan->back = 0;
    char filename[30];
    sprintf( filename, "clanmark/%u.cm", thisclan->logo );
    fh = fopen( filename, "w+b" );
    if(fh==NULL)
    {
        Log( MSG_WARNING, "Error creating clanmark file" );
        return true;
    }
    rewind(fh);
    for(unsigned int i=0;i<P->Size-6;i++)
        fwrite( &P->Buffer[i], 1, 1, fh );
    fclose(fh);        
    DB->QExecute("UPDATE list_clan SET logo=%i,back=0 WHERE id=%i",thisclan->logo, thisclient->clanid );      
	BEGINPACKET( pak, 0x7e1 );//update clan info in world
	ADDBYTE    ( pak, 0xff );
	ADDWORD    ( pak, thisclient->clanid );
	ADDDWORD   ( pak, cmid );
	for(int i=0;i<ChannelList.size();i++)
        send( ChannelList.at(i)->sock, (char*)&pak, pak.Size, 0 );    
    return true;
}

bool CCharServer::pakDownloadCM ( CCharClient* thisclient, CPacket* P )
{
    unsigned int clanid = GETDWORD((*P), 0 );
    MYSQL_RES* result = DB->QStore("SELECT logo FROM list_clan WHERE id=%i",clanid );
    if(result==NULL) return true;
    if(mysql_num_rows(result)!=1) return true;
    MYSQL_ROW row = mysql_fetch_row(result);
    unsigned int cmid = atoi(row[0]);
    DB->QFree( );          
    char filename[30];
    sprintf( filename, "clanmark/%u.cm", cmid ); 
    FILE* fh = fopen( filename, "rb" );
    if(fh==NULL)
    {
        Log( MSG_WARNING, "Invalid clanmark ID %i", cmid );
        return true;
    }
    CClans* thisclan = GetClanByID(thisclient->clanid);
    BEGINPACKET( pak, 0x7e7 );
    ADDDWORD   ( pak, clanid );
    while(!feof(fh))
    {
        unsigned char charvalue = '\0';
        fread( &charvalue, 1 , 1, fh );
        ADDBYTE( pak, charvalue );
    }
    ADDWORD( pak,0);
    thisclient->SendPacket( &pak );
    fclose(fh);
    return true;
}
bool CCharServer::pakClanIconTime ( CCharClient* thisclient, CPacket* P ) //FORMAT UNKNOWN
{
     BEGINPACKET( pak, 0x7e8 );
     ADDWORD    ( pak, 0x07d8); //clanmark change year
     ADDBYTE    ( pak, 0x01  ); //month
     ADDBYTE    ( pak, 0x0e  ); //day
     ADDBYTE    ( pak, 0x12  ); //hour
     ADDBYTE    ( pak, 0x37  ); //minutes
     ADDBYTE    ( pak, 0x00  ); // ?? seconds?
     thisclient->SendPacket( &pak );
    return true;
}
