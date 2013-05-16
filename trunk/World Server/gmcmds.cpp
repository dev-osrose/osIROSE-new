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

    developed with Main erose/hrose source server + some change from the original eich source
*/
#include "worldserver.h"


// Parse our commands to their appropriate function
bool CWorldServer::pakGMCommand( CPlayer* thisclient, CPacket* P )
{

    char* tmp;
    char* command = strtok( (char*)&(*P).Buffer[1] , " ");
    if (command==NULL) return true;
    if (strcmp(command, "here")==0)
    {
        if (Config.Command_Here > thisclient->Session->accesslevel)
            return true;

        return pakGMTele(thisclient, thisclient->Position->Map, thisclient->Position->current.x, thisclient->Position->current.y);
    }
    else if (strcmp(command, "tele")==0) // **** TELEPORT TO MAX AND X Y POINTS *****
    {
        if (Config.Command_Tele > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned map=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        float x=(float)atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        float y=(float)atoi(tmp);
        Log( MSG_GMACTION, " %s : /tele %i,%i,%i" , thisclient->CharInfo->charname, map, x, y);
        return pakGMTele(thisclient, map, x, y);
    }
    else if (strcmp(command, "mute")==0) //==== Mute a player ==== [by Paul_T]
    {
        if (Config.Command_Mute > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        char* id=tmp;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        int min=atoi(tmp);
        Log( MSG_GMACTION, " %s : /mute %s %i" , thisclient->CharInfo->charname, id, min);
        return pakGMMute(thisclient, id, min);
    }
    else if (strcmp(command, "save")==0) // *** SAVE USER DATA *****
    {
        if (Config.Command_Save > thisclient->Session->accesslevel)
            return true;
        thisclient->savedata();
        return true;
    }
    else if (strcmp(command, "reload")==0) // *** REALOAD CONFIG.INI ******
    {
        if (Config.Command_Reload > thisclient->Session->accesslevel)
            return true;
        LoadConfigurations( (char*)filename.c_str()  );
        Log( MSG_GMACTION, " %s : /reload" , thisclient->CharInfo->charname);
        return true;
    }
    else if (strcmp(command, "mystat")==0)
    {
        if ((tmp = strtok(NULL, " "))==NULL)return true;
        if (strcmp(tmp, "ap")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My Attack Power is %i", thisclient->Stats->Attack_Power );
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "acc")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My Accuracy is %i", thisclient->Stats->Accury );
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "hp")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My current HP is %i", thisclient->Stats->HP );
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "mp")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My current MP is %i", thisclient->Stats->MP );
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "maxhp")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My Maximum HP is %i", thisclient->GetMaxHP());
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "maxmp")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My maximum MP is %i", thisclient->GetMaxMP());
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "dodge")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My dodge is %i", thisclient->Stats->Dodge);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "def")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My defense is %i", thisclient->Stats->Defense);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "mdef")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My Magic defense is %i", thisclient->Stats->Magic_Defense);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "crit")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My critical is %i", thisclient->Stats->Critical);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "mspd")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My move speed is %i", thisclient->Stats->Move_Speed);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "aspd")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My attack speed is %i", thisclient->Stats->Attack_Speed);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "xprate")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My xp rate is %i", thisclient->Stats->xprate);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "stamina")==0)
        {
            char buffer2[200];
            sprintf ( buffer2, "My Stamina is %i", thisclient->CharInfo->stamina);
            SendPM(thisclient, buffer2);
        }
        else if (strcmp(tmp, "weight")==0)
        {
            int weight = thisclient->GetCurrentWeight();
            int maxweight = thisclient->GetMaxWeight();
            char buffer2[200];
            sprintf ( buffer2, "My current weight is %i. max weight = %i", weight,maxweight);
            SendPM(thisclient, buffer2);
        }
    }
    else if (strcmp(command, "ann")==0) // *** SEND A ANNUNCEMENT ***
    {
        if (Config.Command_Ann > thisclient->Session->accesslevel)
            return true;
        Log( MSG_GMACTION, " %s : /ann %s" , thisclient->CharInfo->charname, &P->Buffer[5] );
        return pakGMAnn(thisclient, P);
    }
    /*else if(strcmp(command, "bodysize")==0)
    {
        if ((tmp = strtok(NULL, " ")) == NULL) return true; UINT size=atoi(tmp);
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 36);
        ADDWORD(pak, size);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        SendSysMsg(thisclient, "Body size changed!");
    }*/
    /*else if(strcmp(command, "headsize")==0)
    {
        if ((tmp = strtok(NULL, " ")) == NULL) return true; UINT size=atoi(tmp);
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 35);
        ADDWORD(pak, size);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        SendSysMsg(thisclient, "Head size changed!");
    }*/
    else if (strcmp(command, "face")==0)
    {
        if (Config.Command_Face > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " ")) == NULL) return true;
        UINT face=atoi(tmp);
        if (face > 12) return true;
        UINT style;
        if ((tmp = strtok(NULL, " ")) == NULL) style = 0;
        else style = atoi(tmp);
        if (style > 3) style = 3;
        thisclient->CharInfo->Face = ((face*7) + style + 1);
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 8);
        ADDWORD(pak, thisclient->CharInfo->Face);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        SendPM(thisclient, "Face changed!");
        DB->QExecute("UPDATE characters SET face=%i WHERE id=%i", thisclient->CharInfo->Face, thisclient->CharInfo->charid);
    }
    else if (strcmp(command, "hair")==0)
    {
        if (Config.Command_Hair > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " ")) == NULL) return true;
        UINT hair=atoi(tmp);
        if (hair > 6) return true;
        thisclient->CharInfo->Hair = (hair*5);
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 9);
        ADDWORD(pak, thisclient->CharInfo->Hair);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        DB->QExecute("UPDATE characters SET hairStyle=%i WHERE id=%i", thisclient->CharInfo->Hair, thisclient->CharInfo->charid);
        SendPM(thisclient, "Hair changed!");
    }
    else if (strcmp(command, "pvp")==0)
    {
        if (Config.Command_Pvp > thisclient->Session->accesslevel)
            return true;
        CMap* map = MapList.Index[thisclient->Position->Map];
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 34);
        if (map->allowpvp!=0)
        {
            map->allowpvp = 0;
            ADDWORD(pak, 2);
            SendSysMsg(thisclient, "PVP off");
        }
        else
        {
            map->allowpvp = 1;
            ADDWORD(pak, 51);
            SendSysMsg(thisclient, "PVP on");
        }
        ADDWORD(pak, 0);
        SendToMap(&pak, thisclient->Position->Map);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        SendToMap(&pak, thisclient->Position->Map);
    }
    else if (strcmp(command, "who")==0)
    {
        if (Config.Command_Who > thisclient->Session->accesslevel)
            return true;
        int count=1;
        int gmcount=0;
        int playercount=0;
        char line0[200];
        while (count <= (ClientList.size()-1))
        {
            CPlayer* whoclient = (CPlayer*)ClientList.at(count)->player;
            if (whoclient->isInvisibleMode != true)
            {
                if (whoclient->Session->accesslevel > 100)
                {
                    gmcount++;
                }
                else
                {
                    playercount++;
                }
            }
            count++;
        }
        sprintf(line0, "There are currently %i players and %i game moderators currently connected.", playercount, gmcount);
        Log( MSG_GMACTION, " %s : /who" , thisclient->CharInfo->charname);
        SendPM(thisclient, line0 );
        return true;
    }
 
    else if (strcmp(command, "go")==0) // Use SQL by Likol
    {
 
        if(Config.Command_Go > thisclient->Session->accesslevel)
        {
        DB->QFree( );
        return true;
        }
        if ((tmp = strtok(NULL, " ")) == NULL) tmp = 0;
        int loc = atoi(tmp);
        int x = 0;
        int y = 0;
        int map = 0;
        MYSQL_ROW row;
        MYSQL_RES *result = NULL;
        result = DB->QStore("SELECT lvlmin,map,locx,locy,mapname,lvlmax FROM list_golist WHERE isactive=1 AND loc=%i",loc);
        row = mysql_fetch_row(result);
        if (row==NULL)
        {
            SendPM(thisclient, "Please input a number after the go command, below is a list of places and their appropriate number");
            DB->QFree( );
            result = DB->QStore("SELECT loc,mapname FROM list_golist WHERE isactive=1");
            while(row = mysql_fetch_row(result)) SendPM(thisclient, "%i = %s",atoi(row[0]),row[1]);
            SendPM(thisclient, "Example; /go 1");
            DB->QFree( );
            return true;
        }
        else
        {
            if (thisclient->Stats->Level<atoi(row[0]))
            {
                SendPM(thisclient, "You need to be a least Level %i to visit %s!",atoi(row[0]),row[4]);
                DB->QFree( );
                return true;
            }
            if (thisclient->Stats->Level>atoi(row[5]))
            {
                SendPM(thisclient, "You need to be between Level %i and %i to visit %s !",atoi(row[0]),atoi(row[5]),row[4]);
                DB->QFree( );
                return true;
            }
            if ( thisclient->Stats->HP < (thisclient->Stats->MaxHP / 2) || thisclient->Stats->HP < 1 || thisclient->Session->inGame == false )
            {
                    SendPM(thisclient, "You need at least 50% HP in order to warp");
                    DB->QFree( );
                    return true;
            }
       fPoint coord;
            int map = atoi(row[1]);
            coord.x = atoi(row[2]);
            coord.y = atoi(row[3]);
            SendPM(thisclient, "teleport to map: %i",map);
            MapList.Index[map]->TeleportPlayer( thisclient, coord, false );
            Log( MSG_GMACTION, " %s : /go %i" , thisclient->CharInfo->charname, loc);
            DB->QFree( );
            return true;
 
        }
    }
 
/*
    else if (strcmp(command, "go")==0) // AtCommandGo
    {
        if (Config.Command_Go > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " ")) == NULL) tmp = 0;
        int loc=atoi(tmp);
        int x = 0;
        int y = 0;
        int map = 0;
        if (loc == 1)
        {
            map = 22;
            x = 5749;
            y = 5101;
        }
        else if (loc == 2)
        {
            map = 1;
            x = 5240;
            y = 5192;
        }
        else if (loc == 3)
        {
            map = 2;
            x = 5516;
            y = 5236;
        }
        else if (loc == 4)
        {
            map = 51;
            x = 5357;
            y = 5013;
        }
        else if (loc == 5)
        {
            map = 6;
            x = 5243;
            y = 5240;
        }
        else if (loc == 6)
        {
            map = 24;
            x = 5525;
            y = 5376;
        }
        else if (loc == 7)
        {
            map = 31;
            x = 5516;
            y = 5437;
        }
        else if (loc == 73)
        {
            map = 33;
            x = 5695;
            y = 5269;
        }
        else if (loc == 8)
        {
            map = 26;
            x = 5681;
            y = 5102;
        }
        else if (loc == 9)
        {
            map = 28;
            x = 5639;
            y = 4761;
        }
        else
        {
            SendPM(thisclient, "Please input a number after the go command, below is a list of places and their appropriate number");
            SendPM(thisclient, "1 = Adventurers plains");
            SendPM(thisclient, "2 = The city of Zant");
            SendPM(thisclient, "3 = Junon Polis");
            SendPM(thisclient, "4 = The city of Eucar");
            SendPM(thisclient, "5 = Training grounds");
            SendPM(thisclient, "6 = El Verloon Desert");
            SendPM(thisclient, "7 = Goblin Cave (B1)(73(B3))");
            SendPM(thisclient, "8 = Forest of Wisdom");
            SendPM(thisclient, "9 = Gorge of Silence");
            SendPM(thisclient, "Example; /go 3");
        }

        if ( (x != 0) && (y != 0) && (map != 0) )
        {
            fPoint coord;
            coord.x = x;
            coord.y = y;
            MapList.Index[map]->TeleportPlayer( thisclient, coord, false );
            Log( MSG_GMACTION, " %s : /go %i" , thisclient->CharInfo->charname, loc);
        }
        return true;
    }
*/

    //******************************* START RESPAWN ***************************
    else if (strcmp(command, "SSPAWN")==0)
    {
        //STARTPOINT IDMOB CANTMIN CANTMAX RESPAWNTIME(s)  (3 points minim)
        if (Config.Command_SSpawn > thisclient->Session->accesslevel)
            return true;
        thisclient->GMRespawnPoints.map = thisclient->Position->Map;
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;
            return true;
        }
        thisclient->GMRespawnPoints.mobID=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;
            return true;
        }
        thisclient->GMRespawnPoints.min=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;
            return true;
        }
        thisclient->GMRespawnPoints.max=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;
            return true;
        }
        thisclient->GMRespawnPoints.n=0;
        thisclient->GMRespawnPoints.respawntime=atoi(tmp);
        thisclient->GMRespawnPoints.b=1;
        BEGINPACKET( pak, 0x702 );
        ADDSTRING( pak, "STARTPOINT" );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket(&pak);
        thisclient->GMRespawnPoints.d=0;
        return true;
    }
    else if (strcmp(command, "SET")==0)
    {
        if (Config.Command_Set > thisclient->Session->accesslevel)
            return true;
        if (thisclient->GMRespawnPoints.b==1 && thisclient->GMRespawnPoints.n<50)
        {
            int n=thisclient->GMRespawnPoints.n;
            thisclient->GMRespawnPoints.points[n].x=floor(thisclient->Position->destiny.x);
            thisclient->GMRespawnPoints.points[n].y=floor(thisclient->Position->destiny.y);
            thisclient->GMRespawnPoints.n++;
            char text[10];
            sprintf(text,"--POINT #%i",thisclient->GMRespawnPoints.n);
            BEGINPACKET( pak, 0x702 );
            ADDSTRING( pak,text );
            ADDBYTE( pak, 0 );
            thisclient->client->SendPacket(&pak);
            return true;
        }
        return true;
    }
    else if (strcmp(command, "ESPAWN")==0)
    {
        if (Config.Command_ESpawn > thisclient->Session->accesslevel)
            return true;
        if (thisclient->GMRespawnPoints.n>3 && thisclient->GMRespawnPoints.b==1)
        {
            if ((tmp = strtok(NULL, " "))==NULL)
                return true;
            int id=atoi(tmp);
            if ((tmp = strtok(NULL, " "))==NULL)
                return true;
            int agressive=atoi(tmp);
            int n=thisclient->GMRespawnPoints.n;
            char points[1000] = "";
            sprintf(&points[0],"%i",n);
            for (int i=0;i<n;i++)
            {
                sprintf(&points[strlen(points)],"|%.0f,%.0f",thisclient->GMRespawnPoints.points[i].x,thisclient->GMRespawnPoints.points[i].y);
            }
            DB->QExecute("INSERT into list_spawnareas (id,map,montype,respawntime,points,min,max,agressive) values (%i,%i,%i,%i,'%s',%i,%i,%i)",
                         id,thisclient->GMRespawnPoints.map, thisclient->GMRespawnPoints.mobID,thisclient->GMRespawnPoints.respawntime,
                         points,thisclient->GMRespawnPoints.min,thisclient->GMRespawnPoints.max,agressive);
            thisclient->GMRespawnPoints.b=0;
            thisclient->GMRespawnPoints.d=1;
            BEGINPACKET( pak, 0x702 );
            ADDSTRING( pak,"ENDSPAWN" );
            ADDBYTE( pak, 0 );
            thisclient->client->SendPacket(&pak);
            ReloadMobSpawn(thisclient, id );
            return true;
        }
        return true;
    }
    else if (strcmp(command, "DSPAWN")==0)
    {
        if (Config.Command_DSpawn > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        int id=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        thisclient->GMRespawnPoints.mobID=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        thisclient->GMRespawnPoints.min=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        thisclient->GMRespawnPoints.max=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        thisclient->GMRespawnPoints.respawntime=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        int agressive = atoi(tmp);
        if (thisclient->GMRespawnPoints.n>3 && thisclient->GMRespawnPoints.d==1)
        {
            int n=thisclient->GMRespawnPoints.n;
            char points[1000] = "";
            sprintf(&points[0],"%i",n);
            for (int i=0;i<n;i++)
            {
                sprintf(&points[strlen(points)],"|%.0f,%.0f",thisclient->GMRespawnPoints.points[i].x,thisclient->GMRespawnPoints.points[i].y);
            }
            DB->QExecute("INSERT into list_spawnareas (id,map,montype,respawntime,points,min,max,agressive) values (%i,%i,%i,%i,'%s',%i,%i,%i)",
                         id,thisclient->GMRespawnPoints.map,
                         thisclient->GMRespawnPoints.mobID,thisclient->GMRespawnPoints.respawntime,
                         points,thisclient->GMRespawnPoints.min,thisclient->GMRespawnPoints.max,agressive);
            thisclient->GMRespawnPoints.b=0;
            BEGINPACKET( pak, 0x702 );
            ADDSTRING( pak, "DUPESPAWN" );
            ADDBYTE( pak, 0 );
            thisclient->client->SendPacket(&pak);
            ReloadMobSpawn(thisclient, id );
            return true;
        }
        return true;
    }
    else if (strcmp(command, "DELETESPAWN")==0)
    {
        if (Config.Command_DelSpawn > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        int id=atoi(tmp);
        DB->QExecute("DELETE from list_spawnareas where id=%i",id);
        BEGINPACKET( pak, 0x702 );
        ADDSTRING( pak, "DELETESPAWN" );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket(&pak);
        CSpawnArea* thisspawn = GetSpawnArea( id );
        if (thisspawn==NULL)
            return true;
        CMap* map = MapList.Index[thisspawn->map];
        for (UINT i=0;i<map->MonsterList.size();i++)
        {
            CMonster* thismon = map->MonsterList.at(i);
            BEGINPACKET( pak, 0x794 );
            ADDWORD    ( pak, thismon->clientid );
            SendToVisible( &pak, thismon );
            MapList.Index[thisspawn->map]->DeleteMonster( thismon );
        }
        DeleteSpawn( thisspawn );
        Log( MSG_GMACTION, " %s : /deletespawn %i" , thisclient->CharInfo->charname, id);
    }//******************************** FINISH RESPAWN ***************************
    else if (strcmp(command, "p")==0) //*** READ THE PACKET.TXT AND SEND IT
    {
        if (Config.Command_Pak > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        char buffer;
        std::string filename = "packet/packet";
        filename += tmp;
        filename += ".txt";
        FILE *packet1 = fopen(filename.c_str(),"r");
        if (packet1==NULL)
        {
            printf("Error opening packet.txt!\n");
            return true;
        }
        unsigned int command = 0;
        fread( &command, 1, 2, packet1 );
        BEGINPACKET(pak,command);
        while ((fscanf(packet1,"%c",&buffer))!=EOF)
            ADDBYTE(pak,buffer);
        thisclient->client->SendPacket( &pak );
        fclose(packet1);
    }
    else if (strcmp(command, "level")==0)
    {
        if (Config.Command_Level > thisclient->Session->accesslevel)
            return true;
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned level=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) name = thisclient->CharInfo->charname;
        else name = tmp;
        Log( MSG_GMACTION, " %s : /level %i %s" , thisclient->CharInfo->charname, level, name);
        return pakGMLevel( thisclient, level, name );
    }
    else if (strcmp(command, "info")==0)
    {
        if (Config.Command_Info > thisclient->Session->accesslevel)
            return true;
        Log( MSG_GMACTION, " %s : /info" , thisclient->CharInfo->charname);
        thisclient->GetPlayerInfo( );
        return true;
    }
    else if (strcmp(command, "exp")==0)
    {
        if (Config.Command_Exp > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned exp=atoi(tmp);
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL)
            name = thisclient->CharInfo->charname;
        else
            name = tmp;
        CPlayer* otherclient = GetClientByCharName( name );
        if (otherclient == NULL)
            return true;
        otherclient->CharInfo->Exp += exp;
        BEGINPACKET( pak, 0x79b );
        ADDDWORD   ( pak, otherclient->CharInfo->Exp );
        ADDWORD    ( pak, otherclient->CharInfo->stamina );
        ADDWORD    ( pak, 0 );
        otherclient->client->SendPacket( &pak );
        Log( MSG_GMACTION, " %s : /exp %i %s" , thisclient->CharInfo->charname, exp, name);
    }
    else if (strcmp(command, "mon")==0)
    {
        if (Config.Command_Mon > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned montype=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned moncount=atoi(tmp);
        Log( MSG_GMACTION, " %s : /mon %i,%i" , thisclient->CharInfo->charname, montype, moncount);
        return pakGMMon( thisclient, montype, moncount );
    }
    else if (strcmp(command, "kick")==0)
    {
        if (Config.Command_Kick > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        Log( MSG_GMACTION, " %s : /kick %s" , thisclient->CharInfo->charname, name);
        return pakGMKick( thisclient, name );
    }
    else if (strcmp(command, "set")==0)
    {
        if (Config.Command_Set > thisclient->Session->accesslevel)
            return true;
        int refine;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        int id=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            refine = 0;
        else
            refine=atoi(tmp);
        refine *= 16;
        BEGINPACKET( pak, 0);
        if (EquipList[2].Index[id]!=NULL)
        {
            thisclient->items[2].itemnum = id;
            thisclient->items[2].itemtype = 2;
            thisclient->items[2].refine =refine;
            thisclient->items[2].durability = 50;
            thisclient->items[2].lifespan = 100;
            thisclient->items[2].count = 1;
            thisclient->items[2].stats = 0;
            thisclient->items[2].socketed = false;
            thisclient->items[2].appraised = true;
            thisclient->items[2].gem = 0;
            thisclient->UpdateInventory( 2 );

            RESETPACKET( pak, 0x7a5);
            ADDWORD( pak, thisclient->clientid );
            ADDWORD( pak, 0x0002);
            ADDWORD( pak, id);	// ITEM NUM
            ADDWORD( pak, BuildItemRefine( thisclient->items[2] ) );	// REFINE
            ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE
            SendToVisible( &pak,thisclient );
        }
        if (EquipList[3].Index[id]!=NULL)
        {
            thisclient->items[3].itemnum = id;
            thisclient->items[3].itemtype = 3;
            thisclient->items[3].refine =refine;
            thisclient->items[3].durability = 50;
            thisclient->items[3].lifespan = 100;
            thisclient->items[3].count = 1;
            thisclient->items[3].stats = 0;
            thisclient->items[3].socketed = false;
            thisclient->items[3].appraised = true;
            thisclient->items[3].gem = 0;
            thisclient->UpdateInventory( 3 );

            RESETPACKET( pak, 0x7a5);
            ADDWORD( pak, thisclient->clientid );
            ADDWORD( pak, 0x0003);
            ADDWORD( pak, id);	// ITEM NUM
            ADDWORD( pak, BuildItemRefine( thisclient->items[3] ));	// REFINE
            ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE
            SendToVisible( &pak,thisclient );
        }
        if (EquipList[4].Index[id]!=NULL)
        {
            thisclient->items[5].itemnum = id;
            thisclient->items[5].itemtype = 4;
            thisclient->items[5].refine =refine;
            thisclient->items[5].durability = 50;
            thisclient->items[5].lifespan = 100;
            thisclient->items[5].count = 1;
            thisclient->items[5].stats = 0;
            thisclient->items[5].socketed = false;
            thisclient->items[5].appraised = true;
            thisclient->items[5].gem = 0;

            thisclient->UpdateInventory( 4 );

            RESETPACKET( pak, 0x7a5);
            ADDWORD( pak, thisclient->clientid );
            ADDWORD( pak, 5);
            ADDWORD( pak, id);	// ITEM NUM
            ADDWORD( pak, BuildItemRefine( thisclient->items[5] ));	// REFINE
            ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE 2602
            SendToVisible( &pak,thisclient );
        }
        if (EquipList[5].Index[id]!=NULL)
        {
            thisclient->items[6].itemnum = id;
            thisclient->items[6].itemtype = 5;
            thisclient->items[6].refine =refine;
            thisclient->items[6].durability = 50;
            thisclient->items[6].lifespan = 100;
            thisclient->items[6].count = 1;
            thisclient->items[6].stats = 0;
            thisclient->items[6].socketed = false;
            thisclient->items[6].appraised = true;
            thisclient->items[6].gem = 0;

            thisclient->UpdateInventory( 6 );

            RESETPACKET( pak, 0x7a5);
            ADDWORD( pak, thisclient->clientid );
            ADDWORD( pak, 6);
            ADDWORD( pak, id);	// ITEM NUM
            ADDWORD( pak, BuildItemRefine( thisclient->items[6] ));	// REFINE
            ADDWORD( pak, thisclient->Stats->Move_Speed);	// REFINE
            SendToVisible( &pak,thisclient );
        }
        thisclient->SetStats( );
    }
    else if (strcmp(command, "cha")==0)
    {
        if (Config.Command_Cha > thisclient->Session->accesslevel)
            return true;
        int slot;
        int tipo;
        int id;
        int stats;
        int socket;
        int refine;
        if ((tmp = strtok(NULL, " "))==NULL)return true;
        if (strcmp(tmp, "mask")==0)
        {
            slot=1;
            tipo=1;
        }
        else if (strcmp(tmp, "cap")==0)
        {
            slot=2;
            tipo=2;
        }
        else if (strcmp(tmp, "suit")==0)
        {
            slot=3;
            tipo=3;
        }
        else if (strcmp(tmp, "back")==0)
        {
            slot=4;
            tipo=6;
        }
        else if (strcmp(tmp, "glov")==0)
        {
            slot=5;
            tipo=4;
        }
        else if (strcmp(tmp, "shoe")==0)
        {
            slot=6;
            tipo=5;
        }
        else if (strcmp(tmp, "weap")==0)
        {
            slot=7;
            tipo=8;
        }
        else if (strcmp(tmp, "shield")==0)
        {
            slot=8;
            tipo=9;
        }
        else
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)return true;
        id=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            stats=0;
        else
            stats=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            socket=0;
        else
            socket=1;
        if ((tmp = strtok(NULL, " "))==NULL)
            refine=0;
        else
            refine=atoi(tmp);
        thisclient->items[slot].itemnum = id;
        thisclient->items[slot].itemtype = tipo;
        thisclient->items[slot].refine = refine;
        thisclient->items[slot].durability = 50;
        thisclient->items[slot].lifespan = 100;
        thisclient->items[slot].count = 1;
        thisclient->items[slot].stats = stats;
        thisclient->items[slot].socketed = socket;
        thisclient->items[slot].appraised = true;
        if (stats > 300)
        {
            thisclient->items[slot].gem = stats;
            thisclient->items[slot].socketed = 1;
        }
        else
        {
            thisclient->items[slot].gem = 0;
        }

        thisclient->items[slot].gem = 0;
        BEGINPACKET( pak, 0x7a5);
        ADDWORD( pak, thisclient->clientid );
        ADDWORD( pak, slot);
        ADDWORD( pak, id);	// ITEM NUM
        ADDWORD( pak, BuildItemRefine(  thisclient->items[slot] ));	// REFINE
        ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE 2602
        SendToVisible( &pak,thisclient );

        thisclient->UpdateInventory( slot );
        thisclient->SetStats( );
        return true;
    }
    else if (strcmp(command, "learnskill")==0)
    {
        if (thisclient->Session->accesslevel < 300)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        UINT skillId = atoi(tmp);
        Log( MSG_GMACTION, " %s : /learnskill %i", thisclient->CharInfo->charname, skillId);
        if (skillId < 0) return false;
        if (skillId > SkillList.size()) return false;
        LearnSkill(thisclient, skillId);
        return true;
    }
    else if (strcmp(command, "refine")==0)
    {
        if (thisclient->Session->accesslevel < 300)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)return true;
        int slot = 7; //defaults to weapon
        int tipo;
        int itemrefine;
        if (strcmp(tmp, "mask")==0)
        {
            slot=1;
        }
        else if (strcmp(tmp, "cap")==0)
        {
            slot=2;
        }
        else if (strcmp(tmp, "suit")==0)
        {
            slot=3;
        }
        else if (strcmp(tmp, "back")==0)
        {
            slot=4;
        }
        else if (strcmp(tmp, "glov")==0)
        {
            slot=5;
        }
        else if (strcmp(tmp, "shoe")==0)
        {
            slot=6;
        }
        else if (strcmp(tmp, "weap")==0)
        {
            slot=7;
        }
        else if (strcmp(tmp, "shield")==0)
        {
            slot=8;
        }
        if ((tmp = strtok(NULL, " "))==NULL)
            itemrefine =0;
        else
            itemrefine = atoi(tmp)<10?atoi(tmp)*16:9*16;
        thisclient->items[slot].refine = itemrefine;

        BEGINPACKET( pak, 0x7a5);
        ADDWORD( pak, thisclient->clientid );
        ADDWORD( pak, slot);
        ADDWORD( pak, thisclient->items[slot].itemnum);	// ITEM NUM
        ADDWORD( pak, BuildItemRefine(  thisclient->items[slot] ));	// REFINE
        ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE 2602
        SendToVisible( &pak,thisclient );

        thisclient->UpdateInventory( slot );
        thisclient->SetStats( );
        return true;
    }
    else if (strcmp(command, "item")==0)//Modified by Hiei (added refine/socket/stats)
    {
        if (Config.Command_Item > thisclient->Session->accesslevel)
            return true;
        UINT itemrefine, itemstats, itemls, itemsocket;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        UINT itemid =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        UINT itemtype =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        UINT itemamount =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            itemrefine =0;
        else
            itemrefine = atoi(tmp)<10?atoi(tmp)*16:9*16;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemls =100;
        else
            itemls = atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            itemsocket =0;
        else
            itemsocket =atoi(tmp)==0?false:true;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemstats =0;
        else
            itemstats =atoi(tmp);
        Log( MSG_GMACTION, " %s : /item %i,%i,%i,%i,%i,%i" , thisclient->CharInfo->charname, itemid, itemtype, itemamount , itemrefine , itemsocket ,itemstats);
        return pakGMItem( thisclient , itemid , itemtype , itemamount , itemrefine , itemls, itemstats , itemsocket );
    }
    else if (strcmp(command, "levelup")==0)
    {
        if (Config.Command_LevelUp > thisclient->Session->accesslevel)
            return true;
        Log( MSG_GMACTION, " %s : /levelup" , thisclient->CharInfo->charname);
        thisclient->CharInfo->Exp += thisclient->GetLevelEXP();
    }
    else if (strcmp(command, "drop")==0)
    {
        if (Config.Command_Drop > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned itemtype=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned itemid=atoi(tmp);
        Log( MSG_GMACTION, " %s : /drop %i,%i" , thisclient->CharInfo->charname, itemtype, itemid);

        CDrop* thisdrop = new CDrop;
        assert(thisdrop);
        thisdrop->clientid = GetNewClientID();
        thisdrop->type = 2;
        thisdrop->pos.x = thisclient->Position->current.x;
        thisdrop->pos.y = thisclient->Position->current.y;
        thisdrop->posMap = thisclient->Position->Map;
        thisdrop->droptime = time(NULL);

        ClearItem(thisdrop->item);
        thisdrop->item.itemnum = itemid;
        thisdrop->item.itemtype = itemtype;
        thisdrop->item.count = 1;
        thisdrop->item.refine = 0;
        thisdrop->item.durability = 35;
        thisdrop->item.lifespan = 100;
        thisdrop->item.appraised = true;
        thisdrop->item.socketed = false;
        thisdrop->item.stats = 0;
        thisdrop->item.gem = 0;

        thisdrop->amount = 1;
        thisdrop->owner = 0;
        CMap* map = MapList.Index[thisdrop->posMap];
        map->AddDrop( thisdrop );
    }
    else if (strcmp(command, "givezuly")==0)
    {
        if (Config.Command_GiveZuly > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        int zuly=atoi(tmp);
        Log( MSG_GMACTION, " %s : /givezuly %s, %i" , thisclient->CharInfo->charname, name, zuly);
        return pakGMZulygive(thisclient, name, zuly);
    }
    else if (strcmp(command, "npc")==0)
    {
        if (Config.Command_Npc > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned npcid=atoi(tmp);
        unsigned npcdialog = 0;
        if ((tmp = strtok(NULL, " "))!=NULL)
            npcdialog=atoi(tmp);
        Log( MSG_GMACTION, " %s : /npc %i, %i" , thisclient->CharInfo->charname, npcid, npcdialog);
        return pakGMNpc(thisclient, npcid, npcdialog);
    }
    else if (strcmp(command, "givefairy")==0)
    {
        if (Config.Command_GiveFairy > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned mode=atoi(tmp);
        Log( MSG_GMACTION, " %s : /givefairy %s, %i" , thisclient->CharInfo->charname, name, mode);
        return pakGMFairyto(thisclient, name, mode);
    }
    else if (strcmp(command, "fairymode")==0)
    {
        if (Config.Command_ManageFairy > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned mode=atoi(tmp);
        Log( MSG_GMACTION, " %s : /fairymode mode: %i" , thisclient->CharInfo->charname, mode);
        return pakGMManageFairy(thisclient, mode);
    }
    else if (strcmp(command, "fairywait")==0)
    {
        if (Config.Command_ChangeFairyWait > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned value=atoi(tmp);
        Log( MSG_GMACTION, " %s : /fairywait value: %i" , thisclient->CharInfo->charname, value);
        return pakGMChangeFairyWait(thisclient, value);
    }
    else if (strcmp(command, "fairystay")==0)
    {
        if (Config.Command_ChangeFairyStay > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned value=atoi(tmp);
        Log( MSG_GMACTION, " %s : /fairystay value: %i" , thisclient->CharInfo->charname, value);
        return pakGMChangeFairyStay(thisclient, value);
    }
    else if (strcmp(command, "fairytestmode")==0)
    {
        if (Config.Command_ChangeFairyTestMode > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned mode=atoi(tmp);
        Log( MSG_GMACTION, " %s : /fairytestmode mode: %i" , thisclient->CharInfo->charname, mode);
        return pakGMChangeFairyTestMode(thisclient, mode);
    }
    else if (strcmp(command, "move")==0)
    {
        if (Config.Command_Move > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned map=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        float x=(float)atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        float y=(float)atoi(tmp);
        Log( MSG_GMACTION, " %s : /move %s,%i,%i,%i" , thisclient->CharInfo->charname, name, map, x, y);
        return pakGMTeleOtherPlayer(thisclient, name, map, x, y);
    }
    else if (strcmp(command, "goto")==0)
    {
        if (Config.Command_Goto > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        Log( MSG_GMACTION, " %s : /goto %s" , thisclient->CharInfo->charname, name);
        return pakGMTeleToPlayer(thisclient, name);
    }
    else if (strcmp(command, "teletome")==0)
    {
        if (Config.Command_TeleToMe > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        Log( MSG_GMACTION, " %s : /teletome %s" , thisclient->CharInfo->charname, name);
        if (strcmp(name,"all")==0)
        {
            return pakGMTeleAllHere(thisclient);
        }
        return pakGMTelePlayerHere(thisclient, name);
    }
    else if (strcmp(command, "CharInfo")==0)
    {
        if (Config.Command_PlayerInfo > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        Log( MSG_GMACTION, " %s : /CharInfo %s" , thisclient->CharInfo->charname, name);
        return pakGMInfo(thisclient, name);
    }
    else if (strcmp(command, "give2")==0)
    {
        if (Config.Command_Item > thisclient->Session->accesslevel)
            return true;
        UINT itemrefine, itemstats, itemls, itemsocket;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        UINT itemid =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        UINT itemtype =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        UINT itemamount =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            itemrefine =0;
        else
            itemrefine = atoi(tmp)<10?atoi(tmp)*16:9*16;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemls =100;
        else
            itemls = atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            itemsocket =0;
        else
            itemsocket =atoi(tmp)==0?false:true;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemstats =0;
        else
            itemstats =atoi(tmp);
        Log( MSG_GMACTION, " %s : /give2 %s,%i,%i,%i,%i,%i,%i,%i" , thisclient->CharInfo->charname, name, itemid , itemtype , itemamount , itemrefine , itemls, itemstats , itemsocket);
        return pakGMItemtoplayer( thisclient , name , itemid , itemtype , itemamount , itemrefine , itemls, itemstats , itemsocket );
    }
    else if (strcmp(command, "ban")==0)
    {
        if (Config.Command_Ban > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* name=tmp;
        Log( MSG_GMACTION, " %s : /ban %s" , thisclient->CharInfo->charname, name);
        return pakGMBan( thisclient, name );
    }
    else if (strcmp(command, "ani")==0)
    {
        if (Config.Command_Ani > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned anid =atoi(tmp);
        Log( MSG_GMACTION, " %s : /ani %i" , thisclient->CharInfo->charname, anid);
        pakGMDoEmote( thisclient, anid );
    }
    else if (strcmp(command,"summon")==0)
    {
        if (Config.Command_Summon > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int summon = atoi(tmp);
        Log( MSG_GMACTION, " %s : /summon %i" , thisclient->CharInfo->charname, summon);
        fPoint position = RandInCircle( thisclient->Position->current, 5 );
        CMap* map = MapList.Index[thisclient->Position->Map];
        map->AddMonster( summon, position, thisclient->clientid );
        //Start Animation
        BEGINPACKET( pak, 0x7b2 );
        ADDWORD    ( pak, thisclient->clientid );
        ADDWORD    ( pak, 2802 );
        SendToVisible( &pak, thisclient );
        //Finish Animation
        RESETPACKET( pak, 0x7bb );
        ADDWORD    ( pak, thisclient->clientid );
        SendToVisible( &pak, thisclient );
        //????
        RESETPACKET( pak, 0x7b9);
        ADDWORD    ( pak, thisclient->clientid);
        ADDWORD    ( pak, 1286 );
        SendToVisible( &pak, thisclient );
        // Add our Mob to the mobs list
    }
    else if (strcmp(command, "shutdown")==0)
    {
        if (Config.Command_Shutdown > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int minutes =atoi(tmp);
        char text[200];
        sprintf( text, "Server will shutdown in %i minutes, Please logout NOW to be sure your information is saved correctly.",
                 minutes );
        BEGINPACKET( pak, 0x702 );
        ADDSTRING  ( pak, thisclient->CharInfo->charname );
        ADDSTRING  ( pak, "> " );
        ADDSTRING  ( pak, text );
        ADDBYTE    ( pak, 0x00 );
        SendToAll  ( &pak );
        pthread_create( &WorldThread[SHUTDOWN_THREAD], NULL, (void *(__cdecl *)(void *))ShutdownServer, (PVOID)minutes);
        Log( MSG_GMACTION, " %s : /shutdown %u" , thisclient->CharInfo->charname, shutdown);
    }
    else if (strcmp(command, "dquest")==0)
    {
        if (Config.Command_DQuest > thisclient->Session->accesslevel)
            return true;
        char buffer2[200];
        if ( thisclient->questdebug )
        {
            thisclient->questdebug = false;
            sprintf ( buffer2, "Quest Debug Mode disabled" );
            SendPM(thisclient, buffer2);
        }
        else
        {
            thisclient->questdebug = true;

            sprintf ( buffer2, "Quest Debug Mode enabled" );
            SendPM(thisclient, buffer2);
        }
        return true;
    }
     else if(strcmp(command, "debugmode")==0)
    {
        if(thisclient->Session->accesslevel <= 100)
	                    return true;
        char buffer2[200];
        if( thisclient->Session->codedebug )
        {
            thisclient->Session->codedebug = false;

            sprintf ( buffer2, "Debug Mode disabled" );
            SendPM(thisclient, buffer2);
        }
        else
        {
            thisclient->Session->codedebug = true;
            sprintf ( buffer2, "Debug Mode enabled" );
            SendPM(thisclient, buffer2);
        }
        return true;
    }
    else if(strcmp(command, "statdebug") == 0)
    {
        if(thisclient->Session->accesslevel <= 100)
        {
            return true;
        }
        if((tmp = strtok(NULL, " ")) == NULL)
        {
            if(thisclient->Session->statdebug != 0)
            {
                thisclient->Session->statdebug = 0;
                SendPM(thisclient, "Debug Mode(ALL STATS) disabled");
            }
            else
            {
                SendPM(thisclient, "Please input a number after the /statdebug command.");
                SendPM(thisclient, "The number can be:");
                SendPM(thisclient, "0: Turn off | 99: All stats");
                SendPM(thisclient, "1:MaxHP | 2:MaxMP | 3:AP, 4:Def | 5:Crit");
                SendPM(thisclient, "6:MS | 7:MD | 8:MA | 9:Hit | 10:Dodge");
                SendPM(thisclient, "11:AS");
            }
            return true;
        }
        unsigned int statid = atoi(tmp);
        switch(statid)
        {
            case 0: //Switch off
                thisclient->Session->statdebug = 0;
                SendPM(thisclient, "Debug Mode disabled");
            break;
            case 1: //MaxHP
                thisclient->Session->statdebug = 1;
                SendPM(thisclient, "Debug Mode(MaxHP) enabled");
            break;
            case 2: //MaxMP
                thisclient->Session->statdebug = 2;
                SendPM(thisclient, "Debug Mode(MaxMP) enabled");
            break;
            case 3: //AP
                thisclient->Session->statdebug = 3;
                SendPM(thisclient, "Debug Mode(AP) enabled");
            break;
            case 4: //Def
                thisclient->Session->statdebug = 4;
                SendPM(thisclient, "Debug Mode(Def) enabled");
            break;
            case 5: //Crit
                thisclient->Session->statdebug = 5;
                SendPM(thisclient, "Debug Mode(Crit) enabled");
            break;
            case 6: //MS
                thisclient->Session->statdebug = 6;
                SendPM(thisclient, "Debug Mode(MS) enabled");
            break;
            case 7: //Magic Defense
                thisclient->Session->statdebug = 7;
                SendPM(thisclient, "Debug Mode(Mag. Def.) enabled");
            break;
            case 8: //Magic Attack
                thisclient->Session->statdebug = 8;
                SendPM(thisclient, "Debug Mode(Mag. Atk.) enabled");
            break;
            case 9: //Accuracy
                thisclient->Session->statdebug = 9;
                SendPM(thisclient, "Debug Mode(Hit) enabled");
            break;
            case 10: //Dodge
                thisclient->Session->statdebug = 10;
                SendPM(thisclient, "Debug Mode(Dodge) enabled");
            break;
            case 11: //Attack Speed
                thisclient->Session->statdebug = 11;
                SendPM(thisclient, "Debug Mode(AS) enabled");
            break;
        }
        return true;
    }
    else if (strcmp(command, "iquest")==0)
    {
        if (Config.Command_IQuest > thisclient->Session->accesslevel)
            return true;
        int n=1;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int itemquest =atoi(tmp);
        if ((tmp = strtok(NULL, " "))!=NULL)
            n=atoi(tmp);
        if ( thisclient->questdebug )
        {
            for (int i=0;i<n;i++)
            {
                BEGINPACKET( pak, 0x731 )
                ADDWORD    ( pak, itemquest );
                thisclient->client->SendPacket( &pak );
            }
        }
        return true;
    }
    else if (strcmp(command, "addquest")==0)
    {

        if (Config.Command_IQuest > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int questnum =atoi(tmp);
        Log( MSG_GMACTION, "add quest %i" , questnum);
        for (int i=0;i<10;i++)
        {
            if (thisclient->quest.quests[i].QuestID != 0) continue;
            thisclient->quest.quests[i].QuestID = questnum;
            thisclient->quest.quests[i].StartTime = clock_t();
            Log( MSG_GMACTION, "quest added %i" , thisclient->quest.quests[i].QuestID);
            break;
        }
    }
    else if (strcmp(command, "listquest")==0)
    {

        if (Config.Command_IQuest > thisclient->Session->accesslevel)
            return true;
        SendPM( thisclient, "Current Quests:" );
        for (int i=0;i<10;i++)
        {
            if (thisclient->quest.quests[i].QuestID == 0) continue;
            SendPM( thisclient, "Quest[%i]: %i[%i] %i[%i] %i[%i] %i[%i] %i[%i]" , thisclient->quest.quests[i].QuestID, thisclient->quest.quests[i].Items[0].itemnum, thisclient->quest.quests[i].Items[0].count, thisclient->quest.quests[i].Items[1].itemnum, thisclient->quest.quests[i].Items[1].count, thisclient->quest.quests[i].Items[2].itemnum,thisclient->quest.quests[i].Items[2].count,thisclient->quest.quests[i].Items[3].itemnum,thisclient->quest.quests[i].Items[3].count,thisclient->quest.quests[i].Items[4].itemnum,thisclient->quest.quests[i].Items[4].count);
        }
    }
    else if (strcmp(command, "listqflag")==0)
    {
        if (Config.Command_IQuest > thisclient->Session->accesslevel)
            return true;
        SendPM(thisclient, "Quest Flags");
        string buffer = "";
        for (dword i = 0; i < 0x40; i++)
        {
            char buf2[5];
            sprintf(buf2, "%i ", thisclient->quest.flags[i]);
            buffer.append(buf2);
            if (i > 0 && i%10 == 0)
            {
                SendPM(thisclient, (char*)buffer.c_str());
                buffer = "";
            }
        }
        SendPM(thisclient, (char*)buffer.c_str());
    }
    else if (strcmp(command, "setqflag")==0)
    {
        if (Config.Command_IQuest > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int var = atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int val = atoi(tmp);
        if (var >= (0x40 * 8)) return true;
        thisclient->quest.SetFlag(var, (val > 0) ? true : false);
        SendPM( thisclient, "Set Flag[%i] = %i", var, val);
    }
    else if (strcmp(command, "listqvar")==0)
    {
        if (Config.Command_IQuest > thisclient->Session->accesslevel)
            return true;
        SendPM( thisclient, "Quest Variables");

        string buffer = "Episode: ";
        for (dword i = 0; i < 5; i++)
        {
            char buf2[5];
            sprintf(buf2, "%02x ", thisclient->quest.EpisodeVar[i]);
            buffer.append(buf2);
        }
        SendPM( thisclient, (char*)buffer.c_str() );

        buffer = "Job: ";
        for (dword i = 0; i < 3; i++)
        {
            char buf2[5];
            sprintf(buf2, "%02x ", thisclient->quest.JobVar[i]);
            buffer.append(buf2);
        }
        SendPM( thisclient, (char*)buffer.c_str());

        buffer = "Planet: ";
        for (dword i = 0; i < 7; i++)
        {
            char buf2[5];
            sprintf(buf2, "%02x ", thisclient->quest.PlanetVar[i]);
            buffer.append(buf2);
        }
        SendPM( thisclient, (char*)buffer.c_str());

        buffer = "Union: ";
        for (dword i = 0; i < 10; i++)
        {
            char buf2[5];
            sprintf(buf2, "%02x ", thisclient->quest.UnionVar[i]);
            buffer.append(buf2);
        }
        SendPM( thisclient, (char*)buffer.c_str());

        for (dword j = 0; j < 10; j++)
        {
            if (thisclient->quest.quests[j].QuestID == 0) continue;
            buffer = "Quest: ";
            for (dword i = 0; i < 10; i++)
            {
                char buf2[5];
                sprintf(buf2, "%02x ", thisclient->quest.quests[j].Variables[i]);
                buffer.append(buf2);
            }
            SendPM( thisclient, (char*)buffer.c_str());
        }
    }
    else if (strcmp(command, "setqvar")==0)
    {
        if (Config.Command_IQuest > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int vartype = atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int var = atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int val = atoi(tmp);
        if (vartype > 3) return true;
        switch (vartype)
        {
        case 0:
            if (var >= 5) return true;
            thisclient->quest.EpisodeVar[var] = val;
            break;
        case 1:
            if (var >= 3) return true;
            thisclient->quest.JobVar[var] = val;
            break;
        case 2:
            if (var >= 7) return true;
            thisclient->quest.PlanetVar[var] = val;
            break;
        case 3:
            if (var >= 10) return true;
            thisclient->quest.UnionVar[var] = val;
            break;
        }
        SendPM( thisclient, "Set Var[%i][%i] = %i", vartype, var, val);
    }
    else if (strcmp(command, "shoptype")==0)
    {
        if (Config.Command_ShopType > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int shoptype =atoi(tmp);
        thisclient->Shop->ShopType = shoptype;
        Log( MSG_GMACTION, " %s : /shoptype %i" , thisclient->CharInfo->charname, shoptype);
    }
    else if (strcmp(command, "stat")==0) /// Code By Minoc
    {
        if (Config.Command_Stat > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* statname =(char*)tmp;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        int statvalue    = atoi(tmp);
        Log( MSG_GMACTION, " %s : /stat %s,%i" , thisclient->CharInfo->charname, statname, statvalue);
        return pakGMStat( thisclient , statname , statvalue);
    }
    else if (strcmp(command, "FLY_MODE")==0) /// just trying some stuff this isn't working(yet?)
    {
        char update[20];
        sprintf(update,"/FLY_MODE ON");
        BEGINPACKET( pak, 0x751 );
        ADDWORD(pak,thisclient->clientid);
        ADDSTRING(pak,update);
        ADDBYTE(pak,1);
        thisclient->client->SendPacket( &pak );
//         delete []update;
        return true;
    }
    else if (strcmp(command, "statp")==0)
    {
        if (Config.Command_Stat > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        int statvalue = atoi(tmp);
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL)
            name = thisclient->CharInfo->charname;
        else
            name = tmp;
        CPlayer* otherclient = GetClientByCharName(name);
        if (otherclient == NULL)
            return true;
        if ((int)otherclient->CharInfo->StatPoints + statvalue < 0)
            otherclient->CharInfo->StatPoints = 0;
        else
            otherclient->CharInfo->StatPoints += statvalue;
        BEGINPACKET( pak, 0x720 );
        ADDWORD( pak, 32 );
        ADDWORD( pak, statvalue );
        ADDWORD( pak, 0 );
        otherclient->client->SendPacket( &pak );
        RESETPACKET( pak, 0x0730 );
        ADDWORD( pak, 5 );
        ADDWORD( pak, 0xa24d );
        ADDWORD( pak, 0x40b3 );
        otherclient->client->SendPacket( &pak );
        Log(MSG_GMACTION, " %s : /stats %i %s", thisclient->CharInfo->charname, statvalue, name);
        return true;
    }
    else if (strcmp(command, "skillp")==0)
    {
        if (Config.Command_Stat > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        int skillp = atoi(tmp);
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL)
            name = thisclient->CharInfo->charname;
        else
            name = tmp;
        CPlayer* otherclient = GetClientByCharName(name);
        if (otherclient == NULL)
            return true;
        if ((int)otherclient->CharInfo->SkillPoints + skillp < 0)
            otherclient->CharInfo->SkillPoints = 0;
        else
            otherclient->CharInfo->SkillPoints += skillp;
        BEGINPACKET( pak, 0x720 );
        ADDWORD( pak, 37 );
        ADDWORD( pak, skillp );
        ADDWORD( pak, 0 );
        otherclient->client->SendPacket( &pak );
        RESETPACKET( pak, 0x0730 );
        ADDWORD( pak, 5 );
        ADDWORD( pak, 0xa24d );
        ADDWORD( pak, 0x40b3 );
        otherclient->client->SendPacket( &pak );
        Log(MSG_GMACTION, " %s : /skillp %i %s", thisclient->CharInfo->charname, skillp, name);
        return true;
    }
    else if (strcmp(command, "killinrange")==0)
    {
        if (Config.Command_KillInRange > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned range=atoi(tmp);
        Log( MSG_GMACTION, " %s : /killinrange %i" , thisclient->CharInfo->charname, range);
        return pakGMKillInRange( thisclient, range );
    }
    else if (strcmp(command, "gotomap")==0) // *** TELEPORT WITH MAP ID *****
    {
        // credits to Blackie
        if (Config.Command_GoToMap > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned map=atoi(tmp);
        Log( MSG_GMACTION, " %s : /gotomap %i" , thisclient->CharInfo->charname, map);
        return pakGMGotomap(thisclient, map);
    }
    else if (strcmp(command, "heal")==0)
    {
        if (Config.Command_Heal > thisclient->Session->accesslevel)
            return true;
        Log( MSG_GMACTION, " %s : /heal", thisclient->CharInfo->charname );
        return pakGMHeal( thisclient );
    }
    else if (strcmp(command, "serverinfo")==0)
    {
        if (Config.Command_ServerInfo > thisclient->Session->accesslevel)
            return true;
        Log( MSG_GMACTION, " %s : /serverinfo" , thisclient->CharInfo->charname);
        return pakGMServerInfo( thisclient );
    }
    else if (strcmp(command, "targetinfo")==0)
    {
        if (Config.Command_TargetInfo > thisclient->Session->accesslevel)
            return true;
        return GMShowTargetInfo( thisclient );
    }
    else if (strcmp(command, "hide")==0)
    {
        if (Config.Command_Hide > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        int mode= atoi( tmp );
        return pakGMHide( thisclient, mode );
    }
    else if (strcmp(command, "class")==0)
    {
        if (Config.Command_Class > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true;
        char* classid = (char*)tmp;
        Log( MSG_GMACTION, " %s : /class %s" , thisclient->CharInfo->charname, classid);
        return pakGMClass( thisclient, classid );
    }
    else if (strcmp(command, "convert")==0)
    {
        if (Config.Command_Convert > thisclient->Session->accesslevel)
            return true;
        if (thisclient->Battle->target==0)
            return true;
        if ((tmp = strtok(NULL, " " ))==NULL)
            return true;
        UINT newmon = atoi(tmp);
        if (newmon==0)
            return true;
        CMonster* thismon = GetMonsterByID( thisclient->Battle->target, thisclient->Position->Map );
        if (thismon==NULL)
        {
            CNPC* thisnpc = GetNPCByID( thisclient->Battle->target, thisclient->Position->Map );
            if (thisnpc==NULL) return true;
            MapList.Index[thisclient->Position->Map]->ConverToMonster( thisnpc, newmon );
        }
        else
        {
            MapList.Index[thisclient->Position->Map]->ConverToMonster( thismon, newmon );
        }
        return true;
    }
    else if (strcmp(command, "b")==0)
    {
        if (Config.Command_Broadcast > thisclient->Session->accesslevel)
            return true;
        time_t seconds;
        seconds = time (NULL);
        if ((thisclient->CharInfo->LastGlobal+Config.Command_GlobalTime) <= seconds or thisclient->Session->accesslevel > 100)
        {
            thisclient->CharInfo->LastGlobal = time (NULL);
            char outputmsg[200];
            sprintf( outputmsg, "%s %s", Config.Command_GlobalPrefix, &P->Buffer[3] );
            Log( MSG_INFO, "%s> %s %s", thisclient->CharInfo->charname, Config.Command_GlobalPrefix, &P->Buffer[3]);
            SendGlobalMSG(thisclient, outputmsg);
        }
        else
        {
            long int remaining = (Config.Command_GlobalTime-(seconds-thisclient->CharInfo->LastGlobal));
            char buffer2[200];
            sprintf ( buffer2, "Please wait %i seconds before sending another global message.", remaining );
            SendPM(thisclient, buffer2);
        }
        return true;
    }
    else if (strcmp(command, "who2")==0)
    {
        if (Config.Command_Who2 > thisclient->Session->accesslevel)
            return true;
        SendPM(thisclient, "The following players are currently connected;");
        int count=1;
        int hiddenam=0;
        char line0[200];
        while (count <= (ClientList.size()-1))
        {
            CPlayer* whoclient = (CPlayer*)ClientList.at(count)->player;
            if (whoclient->Session->accesslevel > 100)
            {
                sprintf(line0, "%s - GM[%i]", whoclient->CharInfo->charname, whoclient->Session->accesslevel);
            }
            else
            {
                sprintf(line0, "%s", whoclient->CharInfo->charname);
            }
            if (whoclient->isInvisibleMode != true)
            {
                SendPM(thisclient, line0 );
            }
            else
            {
                hiddenam++;
            }
            count++;
        }
        sprintf(line0, "There are currently %i players connected!", ((ClientList.size()-1)-hiddenam));
        Log( MSG_GMACTION, " %s : /who2" , thisclient->CharInfo->charname);
        SendPM(thisclient, line0 );
        return true;
    }
    else if (strcmp(command, "rate")==0) //incomplete
    {
        if (Config.Command_Rate > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* type = tmp;
        if (strcmp(type,"drop")==0)
        {
            if ((tmp = strtok(NULL, " "))==NULL) return true;
            Config.DROP_RATE = atoi(tmp);
        }
        else
            if (strcmp(type,"exp")==0)
            {
                if ((tmp = strtok(NULL, " "))==NULL) return true;
                Config.EXP_RATE = atoi(tmp);
            }
        return true;
    }
    else if (strcmp(command, "moveto")==0)
    {
        if (Config.Command_Moveto > thisclient->Session->accesslevel)
            return true;
        fPoint position;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        position.x = atof(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        position.y = atof(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        position.z = atof(tmp);
        pakGMMoveTo( thisclient, position );
    }
    else if (strcmp(command, "settime")==0)
    {
        if (Config.Command_Settime > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int time = atoi(tmp);
        if (MapList.Index[thisclient->Position->Map]!=NULL)
        {
            switch (time)
            {
            case MORNING:
                MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->morningtime;
                SendSysMsg( thisclient, "Time changed!, rejoin to see change [MORNING]" );
                break;
            case DAY:
                MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->daytime;
                SendSysMsg( thisclient, "Time changed!, rejoin to see change [DAY]" );
                break;
            case EVENING:
                MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->eveningtime;
                SendSysMsg( thisclient, "Time changed!, rejoin to see change [EVENING]" );
                break;
            case NIGHT:
                MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->nighttime;
                SendSysMsg( thisclient, "Time changed!, rejoin to see change [NIGHT]" );
                break;
            default:
                SendSysMsg( thisclient, "Time: 0=MORNING | 1=DAY | 2=EVENING | 3=NIGHT" );
            }
        }
    }
    else if (strcmp(command, "transx")==0)
    {
        if (Config.Command_Transx > thisclient->Session->accesslevel)
            return true;
        thisclient->CharInfo->Sex = thisclient->CharInfo->Sex==0?1:0;
        BEGINPACKET( pak, 0x720 );
        ADDWORD( pak, 2 );
        ADDWORD( pak, thisclient->CharInfo->Sex );
        ADDWORD( pak, 0 );
        thisclient->client->SendPacket( &pak );
        RESETPACKET( pak, 0x0730 );
        ADDWORD( pak, 5 );
        ADDWORD( pak, 0xa24d );
        ADDWORD( pak, 0x40b3 );
        thisclient->client->SendPacket( &pak );
    }
    else if (strcmp(command, "partylvl")==0)
    {
        if (Config.Command_Partylvl > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        int level= atoi( tmp );
        return pakGMPartylvl( thisclient, level );
    }
    else if (strcmp(command, "pdmg")==0)
    {
        if (Config.Command_Pdmg > thisclient->Session->accesslevel) return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned rate=atoi(tmp);
        Log( MSG_GMACTION, " Rate for Player Dmg is now set at %i by %s" , rate, thisclient->CharInfo->charname);
        return pakGMChangePlayerDmg(thisclient, rate);
    }
    else if (strcmp(command, "mdmg")==0)
    {
        if (Config.Command_Mdmg > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned rate=atoi(tmp);
        Log( MSG_GMACTION, " Rate for Monster Dmg is now set at %i by %s" , rate, thisclient->CharInfo->charname);
        return pakGMChangeMonsterDmg(thisclient, rate);
    }
    else if (strcmp(command, "rules")==0) // Rules Command by Matt
    {
        if (Config.Command_Rules > thisclient->Session->accesslevel)
            return true;
        SendPM(thisclient, "Please follow the following rules to ensure you have a fun time on this server!");
        SendPM(thisclient, "Rule 1: No hacking");
        SendPM(thisclient, "Rule 2: Do not abuse any bugs in the game");
        SendPM(thisclient, "Rule 3: Be respectful to all players and GMs");
        SendPM(thisclient, "Rule 4: Do not beg for items");
        SendPM(thisclient, "Rule 5: Do not harass any players or GM's");
        SendPM(thisclient, "Rule 6: Do not abuse any bugs in the game");
    }
    else if (strcmp(command, "cfmode")==0)
    {
        if (Config.Command_Cfmode > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned mode=atoi(tmp);
        Log( MSG_GMACTION, " Clan field mode changed to %i by %s" , mode, thisclient->CharInfo->charname);
        return pakGMChangeCfmode(thisclient, mode);
    }
    else if (strcmp(command, "skill")==0)
    {
        if (Config.Command_KillInRange > thisclient->Session->accesslevel)
            return true;

        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned skillid = atoi(tmp);
        Log(MSG_GMACTION, "%s cast skill %i", thisclient->CharInfo->charname, skillid);
        SendPM(thisclient, "Casting skill %i", skillid);
        CCharacter* Target = thisclient->GetCharTarget( );
        CSkills* thisskill = GetSkillByID(skillid);
        thisclient->UseSkill(thisskill, Target);
    }
    else
    {
        Log( MSG_WARNING, "Invalid GM Command '%s' by '%s'", command, thisclient->CharInfo->charname);
    }
    return true;
}

// GM: Announcment
bool CWorldServer::pakGMAnn( CPlayer* thisclient, CPacket *P )
{
    BEGINPACKET( pak, 0x702 );
    ADDSTRING( pak, thisclient->CharInfo->charname );
    ADDSTRING( pak, "> " );
    ADDSTRING( pak, (&P->Buffer[5]));
    ADDBYTE( pak, 0x00);
    SendToAll( &pak );

    return true;
}

// GM: Spawn x mobs
bool CWorldServer::pakGMMon( CPlayer* thisclient, int montype, int moncount )
{
    for (int i=0; i<moncount; i++)
    {
        fPoint position = RandInCircle( thisclient->Position->current, 10 );
        CMap* map = MapList.Index[thisclient->Position->Map];
        map->AddMonster( montype, position, 0, NULL, NULL, 0 , true );
    }
    return true;
}

// GM: Teleport somewhere
bool CWorldServer::pakGMTele( CPlayer* thisclient, int map, float x, float y )
{
    fPoint coord;
    coord.x = x;
    coord.y = y;
    MapList.Index[map]->TeleportPlayer( thisclient, coord, false );
    return true;
}

// [by Paul_T] [Thanks to AridTag for the packet :D]
bool CWorldServer::pakGMMute( CPlayer* thisclient, char* name, int time)
{
    CPlayer* otherclient = GetClientByCharName ( name );
    BEGINPACKET( pak, 0x70d );
    ADDBYTE    ( pak, 0 );
    ADDWORD    ( pak, time );
    ADDSTRING  ( pak, thisclient->CharInfo->charname );
    ADDBYTE    ( pak, 0 );
    otherclient->client->SendPacket( &pak );
    return true;
}

// GM: Item   - Modified by Hiei (added refine/socket/stats)
bool CWorldServer::pakGMItem( CPlayer* thisclient, UINT itemid, UINT itemtype, UINT itemamount, UINT itemrefine, UINT itemls, UINT itemstats, UINT itemsocket )
{
    CItem item;
    item.count            = itemamount;
    item.durability        = 40;
    item.itemnum        = itemid;
    item.itemtype        = itemtype;
    item.lifespan        = itemls;
    item.refine            = itemrefine;
    item.stats            = itemstats;
    item.socketed        = itemsocket;
    item.appraised        = 1;
    item.gem = 0;
    unsigned newslot = thisclient->GetNewItemSlot( item );
    if (newslot != 0xffff)
    {
        thisclient->items[newslot] = item;
        thisclient->UpdateInventory( newslot );
        char buffer[200];
        sprintf( buffer, "Item added! (ID: %i) (Type: %i) (Refine: %i) (Socket: %i)", item.itemnum, item.itemtype, item.refine, item.socketed );
        BEGINPACKET ( pak, 0x702 );
        ADDSTRING( pak, buffer );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );

    }
    else
    {
        BEGINPACKET( pak, 0x7a7 );
        ADDWORD( pak, thisclient->clientid );
        ADDBYTE( pak, 5 );
        thisclient->client->SendPacket( &pak );

        RESETPACKET ( pak, 0x702 );
        ADDSTRING( pak, "No free slot !" );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );
    }
    return true;
}

// GM: Kick
bool CWorldServer::pakGMKick( CPlayer* thisclient, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if (otherclient==NULL)
        return true;
    BEGINPACKET( pak, 0x702 );
    ADDSTRING( pak, "You were disconnected from the server !" );
    ADDBYTE( pak, 0 );
    otherclient->client->SendPacket( &pak );

    RESETPACKET( pak, 0x707 );
    ADDWORD( pak, 0 );
    otherclient->client->SendPacket( &pak );

    otherclient->client->isActive = false;

    return true;
}

// GM: Ban
bool CWorldServer::pakGMBan( CPlayer* thisclient, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if (otherclient==NULL)
        return true;
    otherclient->Session->accesslevel = -1;
    DB->QExecute( "UPDATE accounts SET accesslevel='0' WHERE id=%i", otherclient->Session->userid);
    BEGINPACKET( pak, 0x702 );
    ADDSTRING( pak, "You were banned from the server !" );
    ADDBYTE( pak, 0 );
    otherclient->client->SendPacket( &pak );

    RESETPACKET( pak, 0x707 );
    ADDWORD( pak, 0 );
    otherclient->client->SendPacket( &pak );

    otherclient->client->isActive = false;

    return true;
}

// GM: Add/Remove/Drop/Set zuly
bool CWorldServer::pakGMZuly( CPlayer* thisclient, int mode, int amount, char* charname )
{
    CPlayer* otherclient = GetClientByCharName(charname);
    if (otherclient==NULL)
        return true;
    if (mode == 1)
    {
        //ADD
        otherclient->CharInfo->Zulies += amount;
        BEGINPACKET( pak, 0x71d );
        ADDQWORD( pak, otherclient->CharInfo->Zulies );
        otherclient->client->SendPacket( &pak );
    }
    if (mode == 2)
    {
        //REMOVE
        otherclient->CharInfo->Zulies -= amount;
        BEGINPACKET( pak, 0x71d );
        ADDQWORD( pak, otherclient->CharInfo->Zulies );
        otherclient->client->SendPacket( &pak );
    }
    if (mode == 3)
    {
        //DROP
        CDrop* thisdrop = new CDrop;
        assert(thisdrop);
        thisdrop->clientid = GetNewClientID();
        thisdrop->type = 1; // ZULY
        thisdrop->pos.x = otherclient->Position->current.x;
        thisdrop->pos.y = otherclient->Position->current.y;
        thisdrop->posMap = otherclient->Position->Map;
        thisdrop->droptime = time(NULL);
        thisdrop->amount = amount;
        thisdrop->owner = 0;
        CMap* map = MapList.Index[thisdrop->posMap];
        map->AddDrop( thisdrop );
    }
    if (mode == 4)
    {
        //SET
        otherclient->CharInfo->Zulies = amount;
        BEGINPACKET( pak, 0x71d );
        ADDQWORD( pak, otherclient->CharInfo->Zulies );
        otherclient->client->SendPacket( &pak );
    }
    return true;
}

// Change player Level
bool CWorldServer::pakGMLevel( CPlayer* thisclient, int level, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if(otherclient == NULL)
    {
        return true;
    }
    if(((int)otherclient->Stats->Level + level) > 0)
    {
        otherclient->Stats->Level += level;
    }
    else
    {
        otherclient->Stats->Level = 1;
    }
    if (otherclient->Stats->Level > 250)
    {
        otherclient->Stats->Level = 250;
    }
    otherclient->CharInfo->Exp = 0;
    if(level < 0)
    {
        otherclient->CharInfo->StatPoints = 0;
        otherclient->CharInfo->SkillPoints = 0;
        if(otherclient->Stats->Level > 1)
        {
            for(int s = 2; s <= otherclient->Stats->Level; s++)
            {
                otherclient->CharInfo->StatPoints += int((s * 0.8) + 10);
                otherclient->CharInfo->StatPoints += (s + 2) / 2;
            }
        }
        pakGMStat(otherclient, "str", 15);
        pakGMStat(otherclient, "dex", 15);
        pakGMStat(otherclient, "con", 15);
        pakGMStat(otherclient, "int", 15);
        pakGMStat(otherclient, "cha", 10);
        pakGMStat(otherclient, "sen", 10);
    }
    else
    {
        for (int s = (otherclient->Stats->Level - level + 1); s <= otherclient->Stats->Level; s++)
        {
            otherclient->CharInfo->StatPoints += int((s * 0.8) + 10);
            otherclient->CharInfo->StatPoints += (s + 2) / 2;
        }
    }
    BEGINPACKET(pak, 0x79e);
    ADDWORD(pak, otherclient->clientid);
    ADDWORD(pak, otherclient->Stats->Level);
    ADDDWORD(pak, otherclient->CharInfo->Exp);
    ADDWORD(pak, otherclient->CharInfo->StatPoints);
    ADDWORD(pak, otherclient->CharInfo->SkillPoints);
    otherclient->client->SendPacket(&pak);

    RESETPACKET( pak, 0x79e );
    ADDWORD( pak, otherclient->clientid );
    SendToVisible( &pak, otherclient, false );

    otherclient->SetStats( );
    otherclient->Stats->HP = otherclient->Stats->MaxHP;
    otherclient->Stats->MP = otherclient->Stats->MaxMP;
    return true;
}


// Reload Mob Spawn
bool CWorldServer::ReloadMobSpawn( CPlayer* thisclient, int id )
{
    CSpawnArea* thisspawn = new (nothrow) CSpawnArea;
    if (thisspawn==NULL)
    {
        Log(MSG_WARNING, "Error allocing memory" );
        return true;
    }
    thisspawn->thisnpc = GetNPCDataByID( thisclient->GMRespawnPoints.mobID );
    if (thisspawn->thisnpc==NULL)
    {
        SendSysMsg( thisclient, "Invalid Respawn" );
        return true;
    }
    thisspawn->id = id;
    thisspawn->map = thisclient->GMRespawnPoints.map;
    thisspawn->montype = thisclient->GMRespawnPoints.mobID;
    thisspawn->min = thisclient->GMRespawnPoints.min;
    thisspawn->max = thisclient->GMRespawnPoints.max;
    thisspawn->respawntime = thisclient->GMRespawnPoints.respawntime;
    thisspawn->pcount = thisclient->GMRespawnPoints.n;
    thisspawn->points = thisclient->GMRespawnPoints.points;
    CMap* map = MapList.Index[thisspawn->map];
    for (int j=0; j<thisspawn->max; j++)
    {
        fPoint position = RandInPoly( thisspawn->points, thisspawn->pcount );
        map->AddMonster( thisspawn->montype, position, 0, thisspawn->mobdrop, thisspawn->mapdrop, thisspawn->id, true );
    }
    thisspawn->lastRespawnTime=clock();
    return true;
}

// Teleport To player to other place
bool CWorldServer::pakGMTeleOtherPlayer( CPlayer *thisclient, char* name, int map, float x, float y )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if (otherclient != NULL)
    {
        fPoint coord;
        coord.x = x;
        coord.y = y;
        MapList.Index[map]->TeleportPlayer( otherclient, coord, false );
    }
    else
    {
        SendSysMsg( thisclient, "User does not exist or is not online!" );
    }
    return true;
}

// Teleport Me to player
bool CWorldServer::pakGMTeleToPlayer( CPlayer* thisclient, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if (otherclient != NULL)
    {
        MapList.Index[otherclient->Position->Map]->TeleportPlayer( thisclient, otherclient->Position->current, false );
    }
    else
    {
        SendSysMsg( thisclient, "User does not exist or is not online!" );
    }
    return true;
}

// Teleport Player Here
bool CWorldServer::pakGMTelePlayerHere( CPlayer* thisclient, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if (otherclient != NULL)
    {
        CMap* map = MapList.Index[thisclient->Position->Map];
        map->TeleportPlayer( otherclient, thisclient->Position->current, false );
    }
    else
    {
        SendSysMsg( thisclient, "User does not exist or is not online!");
    }
    return true;
}

// Show Player info
bool CWorldServer::pakGMInfo(CPlayer* thisclient, char* name)
{
    CPlayer* otherclient = GetClientByCharName(name);
    if (otherclient==NULL)
        return true;
    char  buffer[200];
    char* jobname;

    if (otherclient != 0)
    {
        if ((otherclient->CharInfo->Job - 0) == 0)
        {
            jobname = "Visitor";
        }
        else if ((otherclient->CharInfo->Job - 0) == 111)
        {
            jobname = "Soldier";
        }
        else if ((otherclient->CharInfo->Job - 0) == 121)
        {
            jobname = "Knight";
        }
        else if ((otherclient->CharInfo->Job - 0) == 122)
        {
            jobname = "Champion";
        }
        else if ((otherclient->CharInfo->Job - 0) == 211)
        {
            jobname = "Muse";
        }
        else if ((otherclient->CharInfo->Job - 0) == 221)
        {
            jobname = "Mage";
        }
        else if ((otherclient->CharInfo->Job - 0) == 222)
        {
            jobname = "Cleric";
        }
        else if ((otherclient->CharInfo->Job - 0) == 311)
        {
            jobname = "Hawker";
        }
        else if ((otherclient->CharInfo->Job - 0) == 321)
        {
            jobname = "Raider";
        }
        else if ((otherclient->CharInfo->Job - 0) == 322)
        {
            jobname = "Scout";
        }
        else if ((otherclient->CharInfo->Job - 0) == 411)
        {
            jobname = "Dealer";
        }
        else if ((otherclient->CharInfo->Job - 0) == 421)
        {
            jobname = "Bourgeois";
        }
        else if ((otherclient->CharInfo->Job - 0) == 422)
        {
            jobname = "Artisan";
        }
        else
        {
            jobname = "Unknown";
        }

        BEGINPACKET(pak, 0x702);

        sprintf(buffer, "Info about <%s>" , otherclient->CharInfo->charname);
        ADDSTRING(pak, buffer);
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);

        RESETPACKET(pak, 0x702);
        sprintf(buffer, "Account: %s | ID: %i" , otherclient->Session->username, otherclient->Session->userid);
        ADDSTRING(pak, buffer);
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);

        RESETPACKET(pak, 0x702);
        sprintf(buffer, "Level: %i | Job: %s" , (otherclient->Stats->Level-0) , jobname);
        ADDSTRING(pak, buffer);
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);

        RESETPACKET(pak, 0x702);
        sprintf(buffer,"Map: %i , X: %i , Y: %i", (otherclient->Position->Map-0), (int)otherclient->Position->current.x, (int)otherclient->Position->current.y);
        ADDSTRING(pak, buffer);
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);
    }
    else
    {
        BEGINPACKET(pak, 0x702);
        ADDSTRING(pak, "User does not exist or is not online!");
        ADDBYTE(pak, 0)
        thisclient->client->SendPacket(&pak);
    }
    return true;
}

// Add Fairy
bool CWorldServer::pakGMFairyto(CPlayer* thisclient, char* name, int mode)
{
    if (GServer->Config.FairyMode== 0)
    {
        BEGINPACKET(pak, 0x702);
        ADDSTRING(pak, "Fairy feature is de-activated.");
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);
        return true;
    }
    CPlayer* otherclient = GetClientByCharName (name);
    if (otherclient==NULL)
    {
        BEGINPACKET(pak, 0x702);
        ADDSTRING(pak, "User does not exist or is not online.");
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);
        return true;
    }
    if (mode == 0 && otherclient->Fairy == false)
    {
        BEGINPACKET(pak, 0x702);
        ADDSTRING(pak, "User already not fairied.");
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);
        return true;
    }
    if (mode == 1 && otherclient->Fairy == true)
    {
        BEGINPACKET(pak, 0x702);
        ADDSTRING(pak, "User already fairied.");
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);
        return true;
    }
    if (!otherclient->Fairy && mode == 1)
    {
        int FairyIndex=100;
        for (int i=0; i<GServer->Config.FairyMax; i++)
        {
            if (GServer->FairyList.at(i)->assigned == false)
            {
                FairyIndex=i;
                i=GServer->Config.FairyMax;
            }
        }
        if (FairyIndex == 100)
        {
            BEGINPACKET(pak, 0x702);
            ADDSTRING(pak, "No free Fairy.");
            ADDBYTE(pak, 0);
            thisclient->client->SendPacket(&pak);
            return true;
        }
        BEGINPACKET(pak, 0x702);
        ADDSTRING(pak, "User fairied.");
        ADDBYTE(pak, 0);
        thisclient->client->SendPacket(&pak);
        int ListIndex;
        for (int i=0; i<ClientList.size(); i++)
        {
            if (GServer->ClientList.at(i)->player == otherclient)
            {
                ListIndex = i;
                i = GServer->ClientList.size();
            }
        }
        otherclient->Fairy = true;
        otherclient->FairyListIndex = FairyIndex;
        GServer->FairyList.at(FairyIndex)->ListIndex = ListIndex;
        GServer->FairyList.at(FairyIndex)->assigned = true;
        GServer->FairyList.at(FairyIndex)->LastTime = clock();
        GServer->FairyList.at(FairyIndex)->WaitTime = GServer->Config.FairyWait;
        GServer->DoFairyStuff( otherclient, mode );
        otherclient->SetStats();
        Log( MSG_INFO, "HP: %i  MP: %i  ATK: %i   DEF: %i   CRI: %i  MSPD: %i", otherclient->Stats->MaxHP, otherclient->Stats->MaxMP, otherclient->Stats->Attack_Power, otherclient->Stats->Defense, otherclient->Stats->Critical, otherclient->Stats->Move_Speed);
    }
    if (otherclient->Fairy && mode == 0)
    {
        GServer->DoFairyFree(otherclient->FairyListIndex);
        GServer->FairyList.at(otherclient->FairyListIndex)->WaitTime = GServer->Config.FairyWait;
        otherclient->Fairy = false;
        otherclient->FairyListIndex = 0;
        GServer->DoFairyStuff( otherclient, mode );
        BEGINPACKET(pak, 0x702);
        ADDSTRING(pak, "User unfairied.");
        ADDBYTE(pak, 0);
        otherclient->client->SendPacket(&pak);
        otherclient->SetStats();
        Log( MSG_INFO, "HP: %i  MP: %i  ATK: %i   DEF: %i   CRI: %i  MSPD: %i", otherclient->Stats->MaxHP, otherclient->Stats->MaxMP, otherclient->Stats->Attack_Power, otherclient->Stats->Defense, otherclient->Stats->Critical, otherclient->Stats->Move_Speed);
    }
    otherclient->SetStats();
    return true;
}

// Activate de-activate Fairy mode in game
bool CWorldServer::pakGMManageFairy(CPlayer* thisclient, int mode)
{
    BEGINPACKET (pak, 0x702);
    if (mode == 0)
    {
        if (GServer->Config.FairyMode != mode)
        {
            GServer->Config.FairyMode = 0;
            ADDSTRING(pak, "You have de-activated the Fairy mode ingame.");
        }
        else
        {
            ADDSTRING(pak, "The Fairy mode is already de-activated.");
        }
    }
    else
    {
        if (GServer->Config.FairyMode != mode)
        {
            GServer->Config.FairyMode = 1;
            for (int i=0; i<GServer->Config.FairyMax; i++)
            {
                GServer->DoFairyFree(i);
                GServer->FairyList.at(i)->WaitTime = GServer->Config.FairyWait;
            }
            ADDSTRING(pak, "You have activated the Fairy mode ingame.");
        }
        else
        {
            ADDSTRING(pak, "The Fairy mode is already activated.");
        }
    }
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);

    return true;
}

// Change FairyWait
bool CWorldServer::pakGMChangeFairyWait(CPlayer* thisclient, int newvalue)
{
    GServer->Config.FairyWait = newvalue;
    for (int i=0; i<GServer->FairyList.size(); i++)
    {
        GServer->FairyList.at(i)->WaitTime = newvalue;
    }
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the time between each Fairies");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
    return true;
}

// Change FairyStay
bool CWorldServer::pakGMChangeFairyStay(CPlayer* thisclient, int newvalue)
{
    GServer->Config.FairyStay = newvalue;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the time of Fairies for Buffing ");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
    return true;
}

// Change Fairy Test mode.
// 0 -> normal random x * Fairywait
// 1 -> Test mode activated: 1 * Fairywait only
bool CWorldServer::pakGMChangeFairyTestMode(CPlayer* thisclient, int mode)
{
    GServer->Config.FairyTestMode = mode;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the Fairy test mode.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
    return true;
}

// Give Zuly
bool CWorldServer::pakGMZulygive(CPlayer* thisclient, char* name, int zuly)
{
    CPlayer* otherclient = GetClientByCharName (name);
    if (otherclient==NULL)
        return true;
    otherclient->CharInfo->Zulies += zuly;
    BEGINPACKET(pak, 0x7a7);
    ADDWORD(pak, otherclient->clientid);
    ADDWORD(pak, 0);
    ADDBYTE(pak, 0);
    ADDWORD(pak, 0xccdf );//0xccccccdf);
    ADDDWORD(pak, zuly);
    otherclient->client->SendPacket(&pak);

    return true;
}

// Spawn a NPC
bool CWorldServer::pakGMNpc(CPlayer* thisclient, int npcid,int dialogid)
{
    CNPC* thisnpc = new CNPC;
    assert(thisnpc);
    thisnpc->clientid = GetNewClientID();
    thisnpc->dir = 0;
    thisnpc->npctype = npcid;
    thisnpc->pos = thisclient->Position->current;
    thisnpc->posMap = thisclient->Position->Map;
    thisnpc->thisnpc = GetNPCDataByID( npcid );
    if ( thisnpc->thisnpc==NULL )
    {
        delete thisnpc;
        return true;
    }
    thisnpc->thisnpc->dialogid = dialogid;
    CMap* map = MapList.Index[thisclient->Position->Map];
    map->AddNPC( thisnpc );
    return true;
}

// Give Item to Player
bool CWorldServer::pakGMItemtoplayer(CPlayer* thisclient, char* name , UINT itemid, UINT itemtype, UINT itemamount, UINT itemrefine, UINT itemls, UINT itemstats, UINT itemsocket)
{
    CItem item;
    item.count            = itemamount;
    item.durability        = 40;
    item.itemnum        = itemid;
    item.itemtype        = itemtype;
    item.lifespan        = 100; //itemls Set lifespan to 100
    item.refine            = itemrefine;
    item.stats            = itemstats;
    item.socketed        = itemsocket;
    item.appraised        = 1;
    if (itemstats > 300)
    {
        item.gem = itemstats;
    }
    else
    {
        item.gem = 0;
    }

    CPlayer* otherclient = GetClientByCharName ( name );

    if (otherclient != NULL)
    {
        unsigned newslot = otherclient->GetNewItemSlot( item );
        if (newslot != 0xffff)
        {
            otherclient->items[newslot] = item;
            otherclient->UpdateInventory( newslot );

            BEGINPACKET (pak, 0x702);
            ADDSTRING(pak, "You have recieved an item from a GM !");
            ADDBYTE(pak, 0);
            otherclient->client->SendPacket(&pak);

            RESETPACKET (pak, 0x702);
            ADDSTRING(pak, "Item has been given!");
            ADDBYTE(pak, 0);
            thisclient->client->SendPacket(&pak);
        }
        else
        {
            BEGINPACKET (pak, 0x702);
            ADDSTRING(pak, "No free slot !");
            ADDBYTE(pak, 0);
            thisclient->client->SendPacket(&pak);
        }
    }

    return true;
}

// Do Emotion
bool CWorldServer::pakGMDoEmote( CPlayer* thisclient, int emotionid )
{
    ClearBattle( thisclient->Battle );
    BEGINPACKET( pak, 0x781 );
    ADDDWORD( pak, emotionid );
    ADDWORD( pak, thisclient->clientid );
    SendToVisible( &pak, thisclient );
    return true;
}

// Change GM Stats (Coded by Minoc)
bool CWorldServer::pakGMStat( CPlayer* thisclient, char* statname, int statvalue )
{
    int statid;
    if (strcmp( statname, "Str" )==0 || strcmp( statname, "str" )==0)
    {
        thisclient->Attr->Str = statvalue;
        statid = 0;
    }
    else if (strcmp( statname, "Dex" )==0 || strcmp( statname, "dex" )==0)
    {
        thisclient->Attr->Dex = statvalue;
        statid = 1;
    }
    else if (strcmp( statname, "Int" )==0 || strcmp( statname, "int" )==0)
    {
        thisclient->Attr->Int = statvalue;
        statid = 2;
    }
    else if (strcmp( statname, "Con" )==0 || strcmp( statname, "con" )==0)
    {
        thisclient->Attr->Con = statvalue;
        statid = 3;
    }
    else if (strcmp( statname, "Cha" )==0 || strcmp( statname, "cha" )==0)
    {
        thisclient->Attr->Cha = statvalue;
        statid = 4;
    }
    else if (strcmp( statname, "Sen" )==0 || strcmp( statname, "sen" )==0)
    {
        thisclient->Attr->Sen = statvalue;
        statid = 5;
    }
    else if (strcmp( statname, "Lev" )==0 || strcmp( statname, "lev" )==0)
    {
        char update[20];
        thisclient->Stats->Level = statvalue;
        sprintf(update,"/set lev %d",thisclient->Stats->Level);
        BEGINPACKET( pak, 0x751 );
        ADDWORD(pak,thisclient->clientid);
        ADDSTRING(pak,update);
        thisclient->client->SendPacket( &pak );
        thisclient->SetStats( );
//         delete []update;
        return true;
    }
    else
    {
        return true;
    }
    BEGINPACKET( pak, 0x7a9 );
    ADDBYTE( pak, statid );
    ADDWORD( pak, (unsigned short)statvalue );
    thisclient->client->SendPacket( &pak );
    thisclient->SetStats( );
    return true;
}

// GM: Teleport using map id  credits to Blackie
bool CWorldServer::pakGMGotomap( CPlayer* thisclient, int map )
{
    CRespawnPoint* thisrespawn = MapList.Index[map]->GetFirstRespawn( );
    if (thisrespawn==NULL)
    {
        SendSysMsg( thisclient, "This map have no respawn" );
        return true;
    }
    MapList.Index[map]->TeleportPlayer( thisclient, thisrespawn->dest, false );
    return true;
}

// Heal a player ( by rl2171 )
bool CWorldServer::pakGMHeal( CPlayer* thisclient )
{
    thisclient->Stats->HP = thisclient->Stats->MaxHP;
    thisclient->Stats->MP = thisclient->Stats->MaxMP;
    BEGINPACKET( pak, 0x7ec );
    ADDWORD( pak, thisclient->Stats->HP );
    ADDWORD( pak, thisclient->Stats->MP );
    thisclient->client->SendPacket( &pak );
    return true;
}

// GM: Server Information ( by rl2171 ) modified by me
bool CWorldServer::pakGMServerInfo( CPlayer* thisclient )
{
    char buffer[200];
    // Players Online
    BEGINPACKET( pak, 0x0784 );
    ADDSTRING( pak, "[SYS]ServerInfo" );
    ADDBYTE( pak, 0 );
    ADDSTRING( pak, "SERVER INFORMATION" );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Online Players: %i", (int)ClientList.size()-1 );// -1 (we don't count charserver)
    RESETPACKET( pak, 0x0784 );
    ADDSTRING( pak, "[SYS]ServerInfo" );
    ADDBYTE( pak, 0 );
    ADDSTRING( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    // Exp / Zulies / Drop rates
    sprintf( buffer, "Exp %i | Zulies %i | Drops %i", Config.EXP_RATE, Config.ZULY_RATE, Config.DROP_RATE );
    RESETPACKET( pak, 0x0784 );
    ADDSTRING( pak, "[SYS]ServerInfo" );
    ADDBYTE( pak, 0 );
    ADDSTRING( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    // Send map time
    if (MapList.Index[thisclient->Position->Map]!=NULL)
    {
        RESETPACKET( pak, 0x0784 );
        ADDSTRING( pak, "[SYS]ServerInfo" );
        ADDBYTE( pak, 0 );
        switch (MapList.Index[thisclient->Position->Map]->CurrentTime)
        {
        case MORNING:
            sprintf( buffer, "The Time is: Morning[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
            ADDSTRING( pak, buffer );
            break;
        case DAY:
            sprintf( buffer, "The Time is: Day[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
            ADDSTRING( pak, buffer );
            break;
        case EVENING:
            sprintf( buffer, "The Time is: Evening[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
            ADDSTRING( pak, buffer );
            break;
        case NIGHT:
            sprintf( buffer, "The Time is: Night[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
            ADDSTRING( pak, buffer );
            break;
        default:
            sprintf( buffer, "Invalid Time is the End of world [%i]", MapList.Index[thisclient->Position->Map]->MapTime );
            ADDSTRING( pak, buffer );
        }
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );
    }
    return true;
}

// Show Target Info
bool CWorldServer::GMShowTargetInfo( CPlayer* thisclient )
{
    if (thisclient->Battle->target==0) return true;
    char buffer[200];
    CMonster* monster = GetMonsterByID( thisclient->Battle->target, thisclient->Position->Map );
    if (monster==NULL) return true;
    float dist = distance( thisclient->Position->current, monster->Position->current );
    sprintf( buffer, "Target Position: %.4f, %.4f", monster->Position->current.x, monster->Position->current.y );
    BEGINPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Distance: %.0f",	dist );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Target Defense: %i", monster->Stats->Defense );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Target Level: %i", monster->thisnpc->level );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Target Attack Power: %i", monster->Stats->Attack_Power );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Target Attack Speed: %.0f", monster->Stats->Attack_Speed );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Target Move Speed: %i", monster->Stats->Move_Speed );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    sprintf( buffer, "Target HP/MAXHP: %i/%i", monster->Stats->HP, monster->Stats->MaxHP );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
    return true;
}

// GM: Make yourself invisible from tomiz
bool CWorldServer::pakGMHide( CPlayer* thisclient, int mode )
{
    BEGINPACKET( pak, 0x702 );
    if ( mode == 1 )
    {
        thisclient -> isInvisibleMode = true;
        ADDSTRING( pak, "You are now invisible !" );
        Log( MSG_GMACTION, " %s : /hide invisible" , thisclient->CharInfo->charname);
    }
    else
    {
        thisclient -> isInvisibleMode = false;
        ADDSTRING( pak, "You are now visible !" );
        Log( MSG_GMACTION, " %s : /hide visible" , thisclient->CharInfo->charname);
    }
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket ( &pak );
    return true;
}
// GM : Change the party lvl
bool CWorldServer::pakGMPartylvl( CPlayer* partyclient, int level )
{
    if (partyclient->Party->party == NULL) return true;
    if (level < 0 || level >50) return true;
    partyclient->Party->party->PartyLevel = level;
    if ( partyclient->Party->party->PartyLevel == 50)
        partyclient->Party->party->Exp = 0;
    //Send Party Level and Party Exp
    BEGINPACKET( pak, 0x7d4 ); //
    ADDBYTE    ( pak, partyclient->Party->party->PartyLevel);
    ADDDWORD   ( pak, partyclient->Party->party->Exp );
    partyclient->Party->party->SendToMembers( &pak );
    return true;
}

// GM: Kill all mobs in a range of x-Fields
bool CWorldServer::pakGMKillInRange( CPlayer* thisclient, int range )
{
    for (UINT j = 0; j < MapList.Index[thisclient->Position->Map]->MonsterList.size(); j++)
    {
        CMonster* thismon = MapList.Index[thisclient->Position->Map]->MonsterList.at(j);
        if ( IsMonInCircle( thisclient, thismon, (float)range ))
        {
            //Kill the mob
            thismon->Stats->HP = -1;
            BEGINPACKET( pak, 0x799 );
            ADDWORD    ( pak, thismon->clientid );
            ADDWORD    ( pak, thismon->clientid );
            ADDWORD    ( pak, 0x8005);
            //ADDDWORD   ( pak, thismon->thisnpc->hp*thismon->thisnpc->level );
            //ADDDWORD   ( pak, 16 );
            SendToVisible( &pak, thismon );
            CMap* map = MapList.Index[thisclient->Position->Map];
            map->DeleteMonster( thismon );
        }
    }
    return true;
}

// GM: Change Class (from Crash)
bool CWorldServer::pakGMClass( CPlayer* thisclient, char* classid )
{
    int classid_new;

    if ( strcmp ( classid , "Visitor" ) == 0 || strcmp ( classid , "visitor" ) == 0)
    {
        classid_new = 0;
    }
    else if ( strcmp ( classid , "Soldier" ) == 0 || strcmp ( classid , "soldier" ) == 0)
    {
        classid_new = 111;
    }
    else if ( strcmp ( classid , "Knight" ) == 0 || strcmp ( classid , "knight" ) == 0)
    {
        classid_new = 121;
    }
    else if ( strcmp ( classid , "Champion" ) == 0 || strcmp ( classid , "champion" ) == 0)
    {
        classid_new = 122;
    }
    else if ( strcmp ( classid , "Muse" ) == 0 || strcmp ( classid , "muse" ) == 0)
    {
        classid_new = 211;
    }
    else if ( strcmp ( classid , "Mage" ) == 0 || strcmp ( classid , "mage" ) == 0)
    {
        classid_new = 221;
    }
    else if ( strcmp ( classid , "Cleric" ) == 0 || strcmp ( classid , "cleric" ) == 0)
    {
        classid_new = 222;
    }
    else if ( strcmp ( classid , "Hawker" ) == 0 || strcmp ( classid , "hawker" ) == 0)
    {
        classid_new = 311;
    }
    else if ( strcmp ( classid , "Raider" ) == 0 || strcmp ( classid , "raider" ) == 0)
    {
        classid_new = 321;
    }
    else if ( strcmp ( classid , "Scout" ) == 0 || strcmp ( classid , "scout" ) == 0)
    {
        classid_new = 322;
    }
    else if ( strcmp ( classid , "Dealer" ) == 0 || strcmp ( classid , "dealer" ) == 0)
    {
        classid_new = 411;
    }
    else if ( strcmp ( classid , "Bourgeois" ) == 0 || strcmp ( classid , "bourgeois" ) == 0)
    {
        classid_new = 421;
    }
    else if ( strcmp ( classid , "Artisan" ) == 0 || strcmp ( classid , "artisan" ) == 0)
    {
        classid_new = 422;
    }
    else
    {
        return true;
    }
    thisclient->CharInfo->Job = classid_new;
    BEGINPACKET(pak, 0x0721);
    ADDWORD(pak,4);
    ADDWORD(pak, thisclient->CharInfo->Job);
    ADDWORD(pak,0);
    thisclient->client->SendPacket(&pak);
    RESETPACKET(pak, 0x0730);//05 00 2f f5 64 29
    ADDWORD(pak, 5);
    ADDWORD(pak, 0xf52f);//ADDWORD(pak, 0xa24d);
    ADDWORD(pak, 0x2964);//ADDWORD(pak, 0x40b3);
    thisclient->client->SendPacket(&pak);
    SendPM(thisclient, "Class changed!" );

    return true;
}

bool CWorldServer::pakGMTeleAllHere( CPlayer* thisclient )
{
    int count=1;
    while (count <= (ClientList.size()-1))
    {
        CPlayer* otherclient = (CPlayer*)ClientList.at(count)->player;
        if ((otherclient != NULL) && (otherclient != thisclient))
        {
            CMap* map = MapList.Index[thisclient->Position->Map];
            map->TeleportPlayer( otherclient, thisclient->Position->current, false );
        }
        count++;
    }
    return true;
}

bool CWorldServer::pakGMMoveTo( CPlayer* thisclient, fPoint position )
{
    if ( thisclient->Shop->open || (!thisclient->Ride->Drive && thisclient->Ride->Ride) || !thisclient->Status->CanMove )
        return true;
    if ( thisclient->Status->Stance==1 )
        thisclient->Status->Stance=3;
    thisclient->Position->destiny = position;
    ClearBattle( thisclient->Battle );
    BEGINPACKET( pak, 0x79a );
    ADDWORD    ( pak, thisclient->clientid );
    ADDWORD    ( pak, thisclient->Battle->target );
    ADDWORD    ( pak, thisclient->Stats->Move_Speed );
    ADDFLOAT   ( pak, thisclient->Position->destiny.x*100 );
    ADDFLOAT   ( pak, thisclient->Position->destiny.y*100 );
    ADDFLOAT   ( pak, thisclient->Position->destiny.z*100 );
    SendToVisible( &pak, thisclient );
    return true;
}
//change the player dmg rate
bool CWorldServer::pakGMChangePlayerDmg(CPlayer* thisclient, int rate)
{
    GServer->Config.PlayerDmg = rate;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the player dmg rate.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
    return true;
}
//change the monster dmg rate
bool CWorldServer::pakGMChangeMonsterDmg(CPlayer* thisclient, int rate)
{
    GServer->Config.MonsterDmg = rate;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the Monster dmg rate.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
    return true;
}
//change the Cfmode
bool CWorldServer::pakGMChangeCfmode(CPlayer* thisclient, int mode)
{
    if (mode<0) mode = 0;
    if (mode>1) mode = 1;
    GServer->Config.Cfmode = mode;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the Clan Field Mode.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
    return true;
}
