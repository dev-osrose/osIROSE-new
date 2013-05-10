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
#include "player.h"
#include "worldserver.h"

unsigned char GetCharVal( char mychar )
{
	switch( mychar )
	{
		case '0':	return 0;
		case '1':	return 1;
		case '2':	return 2;
		case '3':	return 3;
		case '4':	return 4;
		case '5':	return 5;
		case '6':	return 6;
		case '7':	return 7;
		case '8':	return 8;
		case '9':	return 9;
		case 'a':	return 10;
		case 'b':	return 11;
		case 'c':	return 12;
		case 'd':	return 13;
		case 'e':	return 14;
		case 'f':	return 15;
		case 'A':	return 10;
		case 'B':	return 11;
		case 'C':	return 12;
		case 'D':	return 13;
		case 'E':	return 14;
		case 'F':	return 15;
	}
	return 0;
}

void EncodeBinary( char* encoded, unsigned char* data, unsigned datalen )
{
	for( unsigned short i = 0, j = 0; i < datalen; i++ )
	{
		encoded[ j++ ] = "0123456789ABCDEF"[ data[ i ] >> 4 ];
		encoded[ j++ ] = "0123456789ABCDEF"[ data[ i ] & 0xf ];
	}
	encoded[ datalen * 2 ] = 0;
}

void DecodeBinary( char* encoded, unsigned char* data )
{
	unsigned curbit = 0;
	for( unsigned i = 0; i < (unsigned)strlen(encoded); i+=2 )
	{
		data[curbit] = GetCharVal( encoded[i] ) << 4;
		data[curbit] += GetCharVal( encoded[i+1] );
		curbit++;
	}
}

// Load this characters userdata from the database
bool CPlayer::loaddata( )
{
	MYSQL_ROW row;
	MYSQL_RES *result = GServer->DB->QStore("SELECT `level`,`face`,`hairStyle`,`sex`,`classid`,`zuly`,`str`,`dex`,`_int`, `con`,`cha`,`sen`,`curHp`,`curMp`,`id`,`statp`,`skillp`,`exp`,`stamina`,`quickbar`,`basic_skills`,`askill`,`askilllvl`,`pskill`,`pskilllvl`,`respawnid`,`clanid`,`clan_rank`,`townid` FROM `characters` WHERE `char_name`='%s'", CharInfo->charname);
	if(result==NULL) return false;
	if(mysql_num_rows(result)!=1)
	{
        Log( MSG_WARNING, "Number of user with charname '%s' is %i", CharInfo->charname,mysql_num_rows(result));
        return false;
    }
	row = mysql_fetch_row(result);
	Stats->Level = atoi(row[0]);
	CharInfo->Face = atoi(row[1]);
	CharInfo->Hair = atoi(row[2]);
	CharInfo->Sex = atoi(row[3]);
	CharInfo->Job = atoi(row[4]);
	CharInfo->Zulies = atol(row[5]);
	Attr->Str = atoi(row[6]);
	Attr->Dex = atoi(row[7]);
	Attr->Int = atoi(row[8]);
	Attr->Con = atoi(row[9]);
	Attr->Cha = atoi(row[10]);
	Attr->Sen = atoi(row[11]);
	Stats->HP = atoi(row[12]);
	Stats->MP = atoi(row[13]);
	CharInfo->charid = atoi(row[14]);
	CharInfo->StatPoints = atoi(row[15]);
	CharInfo->SkillPoints = atoi(row[16]);
	CharInfo->Exp = atoi(row[17]);
	CharInfo->stamina = atoi(row[18]);
    Position->respawn = atoi(row[25]);
    Clan->clanid = atoi(row[26]);
    Clan->clanrank = atoi(row[27]);
    Position->saved = atoi(row[28]);
    Stats->xprate = GetXPRate();
	p_skills = 0;
   	for(char i=0;i<32;i++)
       {
             char* tmp=strtok((i==0?row[19]:NULL), ",");
             if (tmp!=NULL) quickbar[i]=atoi(tmp);
              else
            quickbar[i]=0;
       }

	for(char i=0;i<30;i++)
    {
             char* tmp=strtok((i==0?row[20]:NULL), ",");
             if (tmp!=NULL) bskills[i]=atoi(tmp);
              else
            bskills[i]=0;

    }
	for(char i=0;i<30;i++)
    {
              char* tmp=strtok((i==0?row[21]:NULL), ",");
              if (tmp!=NULL) askill[i]=atoi(tmp);
               else
            askill[i]=0;
    }
	for(char i=0;i<30;i++)
    {
             char* tmp=strtok((i==0?row[22]:NULL), ",");
             if (tmp!=NULL) askilllvl[i]=atoi(tmp);
              else
            askilllvl[i]=0;
    }
	for(char i=0;i<30;i++)
    {
             char* tmp=strtok((i==0?row[23]:NULL), ",");
             if (tmp!=NULL) pskill[i]=atoi(tmp);
              else
            pskill[i]=0;
    }
	for(char i=0;i<30;i++)
    {
             char* tmp=strtok((i==0?row[24]:NULL), ",");
             if (tmp!=NULL) pskilllvl[i]=atoi(tmp);
              else
            pskilllvl[i]=0;
    }

    GServer->DB->QFree( );
	result = GServer->DB->QStore("SELECT unionid,union1points,union2points,union3points,union4points,union5points,union6points,union7points,union8points,union9points,union10points FROM characters WHERE char_name='%s'", CharInfo->charname);
	if(result==NULL) return false;
	if(mysql_num_rows(result)!=1)
	{
        Log( MSG_WARNING, "Number of user with charname '%s' is %i", CharInfo->charname,mysql_num_rows(result));
        return false;
    }
	row = mysql_fetch_row(result);
	for (char points=0; points<11;points++)
        Union_s->unionvar[points] = atoi(row[points]);

	GServer->DB->QFree( );
	result = GServer->DB->QStore("SELECT itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem FROM items WHERE owner=%i", CharInfo->charid);
    if(result==NULL) return false;
	while(row = mysql_fetch_row(result))
    {
        if(!GServer->IsValidItem( atoi(row[1]), atoi(row[0]) ) || atoi(row[6])==0)
        {
            Log(MSG_WARNING, "char %s have a invalid or empty item in inventory: %i-%i [%i], this item will be deleted", CharInfo->charname, atoi(row[1]), atoi(row[0]), atoi(row[6]) );
            continue;
        }
		UINT itemnum = atoi(row[5]);
		items[itemnum].itemnum = atoi(row[0]);
		items[itemnum].itemtype = atoi(row[1]);
		items[itemnum].refine = atoi(row[2]);
		items[itemnum].durability = atoi(row[3]);
		items[itemnum].lifespan = atoi(row[4]);
		items[itemnum].count = atoi(row[6]);
		items[itemnum].stats = atoi(row[7]);
		items[itemnum].socketed = (atoi(row[8])==1)?true:false;
		items[itemnum].appraised = (atoi(row[9])==1)?true:false;
		items[itemnum].gem = atoi(row[10])>3999?0:atoi(row[10]);
		//extra code to fix bugged gem items
if(items[itemnum].gem > 0)
{
items[itemnum].stats = items[itemnum].gem;
}

	}
	GServer->DB->QFree( );
	result = GServer->DB->QStore("SELECT itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem FROM storage WHERE owner=%i", Session->userid);
	if(result==NULL) return false;
	nstorageitems = mysql_num_rows(result);
	while(row = mysql_fetch_row(result))
    {
        if(!GServer->IsValidItem( atoi(row[1]), atoi(row[0]) ) || atoi(row[6])==0)
        {
            Log(MSG_WARNING, "char %s have a invalid or empty item in storage: %i%i [%i], this item will be deleted", CharInfo->charname, atoi(row[1]), atoi(row[0]), atoi(row[6]) );
            continue;
        }
		UINT itemnum = atoi(row[5]);
		storageitems[itemnum].itemnum = atoi(row[0]);
		storageitems[itemnum].itemtype = atoi(row[1]);
		storageitems[itemnum].refine = atoi(row[2]);
		storageitems[itemnum].durability = atoi(row[3]);
		storageitems[itemnum].lifespan = atoi(row[4]);
		storageitems[itemnum].count = atoi(row[6]);
		storageitems[itemnum].stats = atoi(row[7]);
		storageitems[itemnum].socketed = (atoi(row[8])==1)?true:false;
		storageitems[itemnum].appraised = (atoi(row[9])==1)?true:false;
		storageitems[itemnum].gem = atoi(row[10]);
		//extra code to fix bugged gem items
if(storageitems[itemnum].gem > 0)
{
storageitems[itemnum].stats = storageitems[itemnum].gem;
}

	}
	GServer->DB->QFree( );
    if(Clan->clanid!=0)
    {
    	result = GServer->DB->QStore("SELECT `logo`,`back`,`name`,`cp`,`grade` FROM `list_clan` where `id`=%i", Clan->clanid);
        if(result==NULL) return false;
    	if(mysql_num_rows(result)!=1)
    	{
            Log(MSG_WARNING, "There are %i clan(s) with id %i", mysql_num_rows(result), Clan->clanid );
    	    Clan->clanid=0;
        }
    	else
    	{
    	    row = mysql_fetch_row(result);
    	    Clan->logo = atoi(row[0]);
    	    Clan->back = atoi(row[1]);
    	    strcpy(Clan->clanname,row[2]);
    	    Clan->CP = atoi(row[3]);
    	    Clan->grade = atoi(row[4]);
        }
    	GServer->DB->QFree( );
    }
    memset(&quest, 0, sizeof(SQuestData));

    result = GServer->DB->QStore("SELECT quests from list_quest where owner=%i",CharInfo->charid );
    if(result!=NULL) //return false;
    {
    	while(row = mysql_fetch_row(result))
        {
    	   if( row[0] != NULL ) memcpy(&quest, row[0], sizeof(SQuestData));
        }
    }
   	GServer->DB->QFree( );
	Session->isLoggedIn = true;
    GServer->DB->QExecute("UPDATE accounts SET online=1 where id=%u", Session->userid );

    // Calculate how many fairies are available online, according to amoutn of players
	int oldFairyMax = GServer->Config.FairyMax;
    GServer->Config.FairyMax = (int)ceil((float)GServer->ClientList.size() / 50.0); //(1 fairy more every 50 player)
	if( oldFairyMax < GServer->Config.FairyMax ){
        CFairy* thisfairy = new (nothrow) CFairy;
        thisfairy->LastTime = clock();
        thisfairy->assigned = 0;
        thisfairy->ListIndex = 0;
        thisfairy->WaitTime = GServer->Config.FairyWait;
        GServer->FairyList.push_back( thisfairy );
    }
	return true;
}

// This is a REALLY bad way of saving the character data, but it works ^^
void CPlayer::savedata( )
{
    lastSaveTime = clock();
    if(Session->userid!=0)
    {
        CMap* map = GServer->MapList.Index[Position->Map];
    	CRespawnPoint* thisrespawn = map->GetNearRespawn( this );
    	CPlayer* thisclient = GServer->GetClientByID( clientid );
    	if(thisrespawn == NULL)
    	   Position->respawn = 4;
    	else
    	   Position->respawn = thisrespawn->id;
	    char quick[1024];
	    char basic[1024];
	    //char sclass[1024];
 	    //char slevel[1024];
         char active[1024];
         char activelvl[1024];
         char pasive[1024];
         char pasivelvl[1024];
    	for(UINT i=0;i<30;i++)
    	{
            if(i==0)
            	sprintf(&basic[i], "%i",bskills[i]);
            else
                sprintf(&basic[strlen(basic)], ",%i",bskills[i]);
        }
        for(UINT i=0;i<30;i++)
    	{
            if(i==0)
            	sprintf(&active[i], "%i",askill[i]);
            else
                sprintf(&active[strlen(active)], ",%i",askill[i]);
        }
        for(UINT i=0;i<30;i++)
    	{
            if(i==0)
            	sprintf(&activelvl[i], "%i",askilllvl[i]);
            else
                sprintf(&activelvl[strlen(activelvl)], ",%i",askilllvl[i]);
        }
        for(UINT i=0;i<30;i++)
    	{
            if(i==0)
            	sprintf(&pasive[i], "%i",pskill[i]);
            else
                sprintf(&pasive[strlen(pasive)], ",%i",pskill[i]);
        }
        for(UINT i=0;i<30;i++)
    	{
            if(i==0)
            	sprintf(&pasivelvl[i], "%i",pskilllvl[i]);
            else
                sprintf(&pasivelvl[strlen(pasivelvl)], ",%i",pskilllvl[i]);
        }
        for(UINT i=0;i<32;i++)
        {
            if(i==0)
            	sprintf(&quick[i], "%i",quickbar[i]);
            else
                sprintf(&quick[strlen(quick)], ",%i",quickbar[i]);
        }
        long int hp = Stats->HP;
    	if(hp<=0)
    	   hp=Stats->MaxHP * 10 / 100;
	   if(Stats->MP<0)
	       Stats->MP=0;
//Save data complete now?? (Pepu)

        GServer->DB->QExecute("UPDATE characters SET level=%i,face=%i,hairStyle=%i,sex=%i,classid=%i,zuly=%i,str=%i,dex=%i,_int=%i, con=%i,cha=%i,sen=%i,curHp=%i,curMp=%i,id=%i,statp=%i,skillp=%i,exp=%i,stamina=%i,quickbar='%s',basic_skills='%s',askill='%s',askilllvl='%s',pskill='%s',pskilllvl='%s',respawnid=%i,clanid=%i,clan_rank=%i,townid=%i WHERE char_name='%s'",
                    Stats->Level, CharInfo->Face, CharInfo->Hair, CharInfo->Sex, CharInfo->Job, CharInfo->Zulies, Attr->Str, Attr->Dex, Attr->Int, Attr->Con, Attr->Cha, Attr->Sen, hp, Stats->MP, CharInfo->charid, CharInfo->StatPoints, CharInfo->SkillPoints,
                    CharInfo->Exp, CharInfo->stamina, quick, basic, active, activelvl, pasive, pasivelvl, Position->respawn, Clan->clanid, Clan->clanrank, Position->saved, CharInfo->charname );

        GServer->DB->QExecute("UPDATE characters SET unionid=%i,union1points=%i,union2points=%i,union3points=%i,union4points=%i,union5points=%i,union6points=%i,union7points=%i,union8points=%i,union9points=%i,union10points=%i WHERE char_name='%s'",
                    Union_s->unionvar[0], Union_s->unionvar[1],Union_s->unionvar[2], Union_s->unionvar[3], Union_s->unionvar[4], Union_s->unionvar[5], Union_s->unionvar[6], Union_s->unionvar[7], Union_s->unionvar[8], Union_s->unionvar[9], Union_s->unionvar[10], CharInfo->charname );


    	if(!GServer->DB->QExecute("DELETE FROM items WHERE owner=%i", CharInfo->charid)) return;
    	for(UINT i=0;i<MAX_INVENTORY;i++)
        {
    		if (items[i].count > 0)
            {
    			GServer->DB->QExecute("INSERT INTO items (owner,itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
    								CharInfo->charid, items[i].itemnum, items[i].itemtype,items[i].refine, items[i].durability,
    								items[i].lifespan, i, items[i].count, items[i].stats, (items[i].socketed?1:0),
    								(items[i].appraised?1:0),items[i].gem );
    		}
    	}
    	if(!GServer->DB->QExecute("DELETE FROM storage WHERE owner=%i", Session->userid)) return;
    	for(UINT i=0;i<160;i++)
        {
    		if (storageitems[i].count > 0)
            {
    			GServer->DB->QExecute("INSERT INTO storage (owner,itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
    								Session->userid, storageitems[i].itemnum, storageitems[i].itemtype,storageitems[i].refine, storageitems[i].durability,
    								storageitems[i].lifespan, i, storageitems[i].count, storageitems[i].stats, (storageitems[i].socketed?1:0),
    								(storageitems[i].appraised?1:0), storageitems[i].gem );//Gem Fix by Irulagain
    		}
    	}
        savequests( this );
		GServer->DB->QExecute("UPDATE accounts SET zulystorage = %i WHERE id = %i", CharInfo->Storage_Zulies, Session->userid);
		Log(MSG_INFO, "Data Saved for char '%s' ", CharInfo->charname );
    }
}

void CPlayer::savequests( CPlayer* thisclient )
{
    char* questBuffer = new char[sizeof(SQuestData)*3 +1];
	mysql_real_escape_string(GServer->DB->Mysql, questBuffer, (const char*)&quest, sizeof(SQuestData));
    GServer->DB->QExecute( "DELETE FROM list_quest WHERE owner=%i",thisclient->CharInfo->charid );
    GServer->DB->QExecute("INSERT INTO list_quest (owner, quests) VALUES(%i,'%s')",
        thisclient->CharInfo->charid, questBuffer);
    delete questBuffer;
}
