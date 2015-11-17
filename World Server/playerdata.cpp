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
	switch ( mychar )
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'a': return 10;
	case 'b': return 11;
	case 'c': return 12;
	case 'd': return 13;
	case 'e': return 14;
	case 'f': return 15;
	case 'A': return 10;
	case 'B': return 11;
	case 'C': return 12;
	case 'D': return 13;
	case 'E': return 14;
	case 'F': return 15;
	}
	return 0;
}

void EncodeBinary( char* encoded, unsigned char* data, unsigned datalen )
{
	for ( unsigned short i = 0, j = 0; i < datalen; i++ )
	{
		encoded[ j++ ] = "0123456789ABCDEF"[ data[ i ] >> 4 ];
		encoded[ j++ ] = "0123456789ABCDEF"[ data[ i ] & 0xf ];
	}
	encoded[ datalen * 2 ] = 0;
}

void DecodeBinary( char* encoded, unsigned char* data )
{
	unsigned curbit = 0;
	for ( unsigned i = 0; i < (unsigned)strlen( encoded ); i += 2 )
	{
		data[ curbit ] = GetCharVal( encoded[ i ] ) << 4;
		data[ curbit ] += GetCharVal( encoded[ i + 1 ] );
		curbit++;
	}
}

// Load this characters userdata from the database
bool CPlayer::loaddata( )
{
	std::unique_ptr< sql::ResultSet > result = GServer->DB->QStore( "SELECT `level`,`face`,`hairStyle`,`sex`,`classid`,`zuly`,`str`,`dex`,`_int`, `con`,`cha`,`sen`,`curHp`,`curMp`,`id`,`statp`,`skillp`,`exp`,`stamina`,`quickbar`,`basic_skills`,`askill`,`askilllvl`,`pskill`,`pskilllvl`,`respawnid`,`clanid`,`clan_rank`,`townid` FROM `characters` WHERE `char_name`='%s'", CharInfo->charname );
	if ( result == NULL )
		return false;

	if ( result->rowsCount() != 1 )
	{
		Log( MSG_WARNING, "Number of user with charname '%s' is %i", CharInfo->charname, result->rowsCount() );
		return false;
	}
	result->next();

	Stats->Level          = result->getInt("level");
	CharInfo->Face        = result->getInt("face");
	CharInfo->Hair        = result->getInt("hairStyle");
	CharInfo->Sex         = result->getInt("sex");
	CharInfo->Job         = result->getInt("classid");
	CharInfo->Zulies      = result->getInt("zuly");
	Attr->Str             = result->getInt("str");
	Attr->Dex             = result->getInt("dex");
	Attr->Int             = result->getInt("_int");
	Attr->Con             = result->getInt("con");
	Attr->Cha             = result->getInt("cha");
	Attr->Sen             = result->getInt("sen");
	Stats->HP             = result->getInt("curHp");
	Stats->MP             = result->getInt("curMp");
	CharInfo->charid      = result->getInt("id");
	CharInfo->StatPoints  = result->getInt("statp");
	CharInfo->SkillPoints = result->getInt("skillp");
	CharInfo->Exp         = result->getInt("exp");
	CharInfo->stamina     = result->getInt("stamina"); //18
	Position->respawn     = result->getInt("respanid");// 25
	Clan->clanid          = result->getInt("clanid");
	Clan->clanrank        = result->getInt("clan_rank");
	Position->saved       = result->getInt("townid");
	Stats->xprate         = GetXPRate( );
	p_skills              = 0;

//TODO: FIX ALL OF THE (char*)std::string.c_str() calls
	char *next_token;
	for (uint8_t i = 0; i < 32; i++)
	{
		char* tmp = strtok_s( ( i == 0 ? (char*)result->getString("quickbar").c_str() : NULL ), ",", &next_token ); // this is a bad thing to do, but I'm going to do it anyway until I can fix it later
		if ( tmp != NULL )
			quickbar[ i ] = atoi( tmp );
		else
			quickbar[ i ] = 0;
	}

	for ( uint8_t i = 0; i < 30; i++ )
	{
		char* tmp = strtok_s( ( i == 0 ? (char*)result->getString("basic_skills").c_str() : NULL ), ",", &next_token );
		if ( tmp != NULL )
			bskills[ i ] = atoi( tmp );
		else
			bskills[ i ] = 0;
	}
	for ( uint8_t i = 0; i < 30; i++ )
	{
		char* tmp = strtok_s( ( i == 0 ? (char*)result->getString("askill").c_str() : NULL ), ",", &next_token );
		if ( tmp != NULL )
			askill[ i ] = atoi( tmp );
		else
			askill[ i ] = 0;
	}
	for ( uint8_t i = 0; i < 30; i++ )
	{
		char* tmp = strtok_s( ( i == 0 ? (char*)result->getString("askilllvl").c_str() : NULL ), ",", &next_token );
		if ( tmp != NULL )
			askilllvl[ i ] = atoi( tmp );
		else
			askilllvl[ i ] = 0;
	}
	for ( uint8_t i = 0; i < 30; i++ )
	{
		char* tmp = strtok_s( ( i == 0 ? (char*)result->getString("pskill").c_str() : NULL ), ",", &next_token );
		if ( tmp != NULL )
			pskill[ i ] = atoi( tmp );
		else
			pskill[ i ] = 0;
	}
	for ( uint8_t i = 0; i < 30; i++ )
	{
		char* tmp = strtok_s( ( i == 0 ? (char*)result->getString("pskilllvl").c_str() : NULL ), ",", &next_token );
		if ( tmp != NULL )
			pskilllvl[ i ] = atoi( tmp );
		else
			pskilllvl[ i ] = 0;
	}

	result = GServer->DB->QStore( "SELECT unionid,union1points,union2points,union3points,union4points,union5points,union6points,union7points,union8points,union9points,union10points FROM characters WHERE char_name='%s'", CharInfo->charname );
	if ( result == NULL )
		return false;

	if ( result->rowsCount() != 1 )
	{
		Log( MSG_WARNING, "Number of user with charname '%s' is %i", CharInfo->charname, result->rowsCount() );
		return false;
	}

	result->next();
	for ( uint8_t points = 0; points < 11; points++ )
		Union_s->unionvar[ points ] = result->getInt( points );

	result = GServer->DB->QStore( "SELECT itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem FROM items WHERE owner=%i", CharInfo->charid );

	if ( result == NULL )
		return false;

	while ( result->next() )
	{
		if ( !GServer->IsValidItem( result->getInt("itemtype"), result->getInt("itemnum") ) || result->getInt("count") == 0 )
		{
			Log( MSG_WARNING, "char %s have a invalid or empty item in inventory: %i-%i [%i], this item will be deleted", CharInfo->charname, result->getInt("itemtype"), result->getInt("itemnum"), result->getInt("count") );
			continue;
		}
		uint32_t itemnum                = result->getInt("slotnum");
		items[ itemnum ].itemnum    = result->getInt("itemnum");
		items[ itemnum ].itemtype   = result->getInt("itemtype");
		items[ itemnum ].refine     = result->getInt("refine");
		items[ itemnum ].durability = result->getInt("durability");
		items[ itemnum ].lifespan   = result->getInt("lifespan");
		items[ itemnum ].count      = result->getInt("count");
		items[ itemnum ].stats      = result->getInt("stats");
		items[ itemnum ].socketed = ( result->getInt("socketed") == 1 ) ? true : false;
		items[ itemnum ].appraised = ( result->getInt("appraised") == 1 ) ? true : false;
		items[ itemnum ].gem = result->getInt("gem") > 3999 ? 0 : result->getInt("gem");
		//extra code to fix bugged gem items
		if ( items[ itemnum ].gem > 0 )
		{
			items[ itemnum ].stats = items[ itemnum ].gem;
		}
	}
	result = GServer->DB->QStore( "SELECT itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem FROM storage WHERE owner=%i", Session->userid );
	if ( result == NULL )
		return false;

	while ( result->next() )
	{
		if ( !GServer->IsValidItem( result->getInt("itemtype"), result->getInt("itemnum") ) || result->getInt("count") == 0 )
		{
			Log( MSG_WARNING, "char %s have a invalid or empty item in storage: %i%i [%i], this item will be deleted", CharInfo->charname, result->getInt("itemtype"), result->getInt("itemnum"), result->getInt("count") );
			continue;
		}
		uint32_t itemnum                       = result->getInt("slotnum");
		storageitems[ itemnum ].itemnum    = result->getInt("itemnum");
		storageitems[ itemnum ].itemtype   = result->getInt("itemtype");
		storageitems[ itemnum ].refine     = result->getInt("refine");
		storageitems[ itemnum ].durability = result->getInt("durability");
		storageitems[ itemnum ].lifespan   = result->getInt("lifespan");
		storageitems[ itemnum ].count      = result->getInt("count");
		storageitems[ itemnum ].stats      = result->getInt("stats");
		storageitems[ itemnum ].socketed = ( result->getInt("socketed") == 1 ) ? true : false;
		storageitems[ itemnum ].appraised = ( result->getInt("appraised") == 1 ) ? true : false;
		storageitems[ itemnum ].gem = result->getInt("gem");
		//extra code to fix bugged gem items
		if ( storageitems[ itemnum ].gem > 0 )
		{
			storageitems[ itemnum ].stats = storageitems[ itemnum ].gem;
		}
	}

	if ( Clan->clanid != 0 )
	{
		result = GServer->DB->QStore( "SELECT `logo`,`back`,`name`,`cp`,`grade` FROM `list_clan` where `id`=%i", Clan->clanid );
		if ( result == NULL )
			return false;
		if ( result->rowsCount() != 1 )
		{
			Log( MSG_WARNING, "There are %i clan(s) with id %i", result->rowsCount(), Clan->clanid );
			Clan->clanid = 0;
		}
		else
		{
			Clan->logo = result->getInt("logo");
			Clan->back = result->getInt("back");
			strcpy_s( Clan->clanname, 16, result->getString("name").c_str() );
			Clan->CP    = result->getInt("cp");
			Clan->grade = result->getInt("grade");
		}
	}
	memset( &quest, 0, sizeof( SQuestData ) );

	result = GServer->DB->QStore( "SELECT quests from list_quest where owner=%i", CharInfo->charid );
	if ( result != NULL ) //return false;
	{
		while ( result->next() )
		{
			if ( result->getString("quests") != NULL )
				memcpy( &quest, result->getString("quests").c_str(), sizeof( SQuestData ) );
		}
	}

	Session->isLoggedIn = true;
	GServer->DB->QExecute( "UPDATE accounts SET online=1 where id=%u", Session->userid );

	// Calculate how many fairies are available online, according to amoutn of players
	int oldFairyMax          = GServer->Config.FairyMax;
	GServer->Config.FairyMax = (int)ceil( (float)GServer->ClientList.size( ) / 50.0 ); //(1 fairy more every 50 player)
	if ( oldFairyMax < GServer->Config.FairyMax )
	{
		CFairy* thisfairy    = new ( nothrow ) CFairy;
		thisfairy->LastTime  = clock( );
		thisfairy->assigned  = 0;
		thisfairy->ListIndex = 0;
		thisfairy->WaitTime  = GServer->Config.FairyWait;
		GServer->FairyList.push_back( thisfairy );
	}
	return true;
}

// This is a REALLY bad way of saving the character data, but it works ^^
void CPlayer::savedata( )
{
	lastSaveTime = clock( );
	if ( Session->userid != 0 )
	{
		CMap*          map         = GServer->MapList.Index[ Position->Map ];
		CRespawnPoint* thisrespawn = map->GetNearRespawn( this );
		CPlayer*       thisclient  = GServer->GetClientByID( clientid );
(void)thisclient;
		if ( thisrespawn == NULL )
			Position->respawn = 4;
		else
			Position->respawn = thisrespawn->id;
		char quick[ 1024 ];
		char basic[ 1024 ];
		//char sclass[1024];
		//char slevel[1024];
		char active[ 1024 ];
		char activelvl[ 1024 ];
		char pasive[ 1024 ];
		char pasivelvl[ 1024 ];
		for ( uint32_t i = 0; i < 30; i++ )
		{
			if ( i == 0 )
				sprintf_s( &basic[ i ], 1024, "%i", bskills[ i ] );
			else
				sprintf_s( &basic[strlen( basic )], 1024, ",%i", bskills[i] );
		}
		for ( uint32_t i = 0; i < 30; i++ )
		{
			if ( i == 0 )
				sprintf_s( &active[i], 1024, "%i", askill[i] );
			else
				sprintf_s( &active[strlen( active )], 1024, ",%i", askill[i] );
		}
		for ( uint32_t i = 0; i < 30; i++ )
		{
			if ( i == 0 )
				sprintf_s( &activelvl[i], 1024, "%i", askilllvl[i] );
			else
				sprintf_s( &activelvl[strlen( activelvl )], 1024, ",%i", askilllvl[i] );
		}
		for ( uint32_t i = 0; i < 30; i++ )
		{
			if ( i == 0 )
				sprintf_s( &pasive[i], 1024, "%i", pskill[i] );
			else
				sprintf_s( &pasive[strlen( pasive )], 1024, ",%i", pskill[i] );
		}
		for ( uint32_t i = 0; i < 30; i++ )
		{
			if ( i == 0 )
				sprintf_s( &pasivelvl[i], 1024, "%i", pskilllvl[i] );
			else
				sprintf_s( &pasivelvl[strlen( pasivelvl )], 1024, ",%i", pskilllvl[i] );
		}
		for ( uint32_t i = 0; i < 32; i++ )
		{
			if ( i == 0 )
				sprintf_s( &quick[i], 1024, "%i", quickbar[i] );
			else
				sprintf_s( &quick[strlen( quick )], 1024, ",%i", quickbar[i] );
		}
		long int hp = Stats->HP;

		if ( hp <= 0 )
			hp = Stats->MaxHP * 10 / 100;

		if ( Stats->MP < 0 )
			Stats->MP = 0;
		//Save data complete now?? (Pepu)

		GServer->DB->QExecute( "UPDATE characters SET level=%i,face=%i,hairStyle=%i,sex=%i,classid=%i,zuly=%i,str=%i,dex=%i,_int=%i, con=%i,cha=%i,sen=%i,curHp=%i,curMp=%i,id=%i,statp=%i,skillp=%i,exp=%i,stamina=%i,quickbar='%s',basic_skills='%s',askill='%s',askilllvl='%s',pskill='%s',pskilllvl='%s',respawnid=%i,clanid=%i,clan_rank=%i,townid=%i WHERE char_name='%s'",
		                       Stats->Level, CharInfo->Face, CharInfo->Hair, CharInfo->Sex, CharInfo->Job, CharInfo->Zulies, Attr->Str, Attr->Dex, Attr->Int, Attr->Con, Attr->Cha, Attr->Sen, hp, Stats->MP, CharInfo->charid, CharInfo->StatPoints, CharInfo->SkillPoints,
		                       CharInfo->Exp, CharInfo->stamina, quick, basic, active, activelvl, pasive, pasivelvl, Position->respawn, Clan->clanid, Clan->clanrank, Position->saved, CharInfo->charname );

		GServer->DB->QExecute( "UPDATE characters SET unionid=%i,union1points=%i,union2points=%i,union3points=%i,union4points=%i,union5points=%i,union6points=%i,union7points=%i,union8points=%i,union9points=%i,union10points=%i WHERE char_name='%s'",
		                       Union_s->unionvar[ 0 ], Union_s->unionvar[ 1 ], Union_s->unionvar[ 2 ], Union_s->unionvar[ 3 ], Union_s->unionvar[ 4 ], Union_s->unionvar[ 5 ], Union_s->unionvar[ 6 ], Union_s->unionvar[ 7 ], Union_s->unionvar[ 8 ], Union_s->unionvar[ 9 ], Union_s->unionvar[ 10 ], CharInfo->charname );

		if ( !GServer->DB->QExecute( "DELETE FROM items WHERE owner=%i", CharInfo->charid ) )
			return;
		for ( uint32_t i = 0; i < MAX_INVENTORY; i++ )
		{
			if ( items[ i ].count > 0 )
			{
				GServer->DB->QExecute( "INSERT INTO items (owner,itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
				                       CharInfo->charid, items[ i ].itemnum, items[ i ].itemtype, items[ i ].refine, items[ i ].durability,
				                       items[ i ].lifespan, i, items[ i ].count, items[ i ].stats, ( items[ i ].socketed ? 1 : 0 ),
				                       ( items[ i ].appraised ? 1 : 0 ), items[ i ].gem );
			}
		}
		if ( !GServer->DB->QExecute( "DELETE FROM storage WHERE owner=%i", Session->userid ) )
			return;
		for ( uint32_t i = 0; i < 160; i++ )
		{
			if ( storageitems[ i ].count > 0 )
			{
				GServer->DB->QExecute( "INSERT INTO storage (owner,itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
				                       Session->userid, storageitems[ i ].itemnum, storageitems[ i ].itemtype, storageitems[ i ].refine, storageitems[ i ].durability,
				                       storageitems[ i ].lifespan, i, storageitems[ i ].count, storageitems[ i ].stats, ( storageitems[ i ].socketed ? 1 : 0 ),
				                       ( storageitems[ i ].appraised ? 1 : 0 ), storageitems[ i ].gem ); //Gem Fix by Irulagain
			}
		}
		savequests( this );
		GServer->DB->QExecute( "UPDATE accounts SET zulystorage = %i WHERE id = %i", CharInfo->Storage_Zulies, Session->userid );
		Log( MSG_INFO, "Data Saved for char '%s' ", CharInfo->charname );
	}
}

void CPlayer::savequests( CPlayer* thisclient )
{
	char* questBuffer = new char[ sizeof( SQuestData ) * 3 + 1 ];
//TODO: use prepared statements here
	//mysql_real_escape_string( GServer->DB->Mysql, questBuffer, (const char*)&quest, sizeof( SQuestData ) );
	GServer->DB->QExecute( "DELETE FROM list_quest WHERE owner=%i", thisclient->CharInfo->charid );
	GServer->DB->QExecute( "INSERT INTO list_quest (owner, quests) VALUES(%i,'%s')",
	                       thisclient->CharInfo->charid, questBuffer );
	delete questBuffer;
}
