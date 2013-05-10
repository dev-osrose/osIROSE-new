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

// Constructor
CWorldServer::CWorldServer( string fn )
{
    filename = fn;
    LoadConfigurations( (char*)fn.c_str() );
	for(unsigned i=0; i<0xffff; i++)
        ClientIDList[i] = 1;
    //Clear or Vector list
    PartyList.clear();
    MDropList.clear();
    SkillList.clear();
    FairyList.clear();
    TeleGateList.clear();
    //
    CEquip* nullequip = new CEquip;
    nullequip->id = 0;
    nullequip->equiptype = 0;
    nullequip->type = 0;
    nullequip->price = 0;
    nullequip->pricerate = 0;
    nullequip->weight = 0;
    nullequip->quality = 0;
    nullequip->level = 0;
    nullequip->material = 0;
    nullequip->defense = 0;
    nullequip->magicresistence = 0;
    nullequip->attackdistance = 0;
    nullequip->movespeed = 0;
    nullequip->attackpower = 0;
    nullequip->attackspeed =0;
    nullequip->itemgrade = 0;
    for(int i=0;i<3;i++)
    {
        nullequip->occupation[i] = 0;
        nullequip->condition1[i] = 0;
        nullequip->condition2[i] = 0;
    }
    for(int i=0;i<2;i++)
    {
        nullequip->stat1[i] = 0;
        nullequip->stat2[i] = 0;
    }
    for(int i=0;i<10;i++)
    {
        EquipList[i].nullequip = nullequip;
        for(UINT j=0;j<5000;j++)
            EquipList[i].Index[j] = nullequip;
    }
    CNaturalData* nullnatural = new CNaturalData;
    nullnatural->id = 0;
    nullnatural->type = 0;
    nullnatural->price = 0;
    nullnatural->pricerate = 0;
    nullnatural->weight = 0;
    nullnatural->quality = 0;
    nullnatural->pricevalue = 0;
    NaturalList.nullnatural = nullnatural;

    CPatData* nullpat = new CPatData;
    nullpat->id = 0;
    nullpat->type = 0;
    nullpat->price = 0;
    nullpat->pricerate = 0;
    nullpat->weight = 0;
    nullpat->quality = 0;
    nullpat->material = 0;
    nullpat->partversion = 0;
    nullpat->maxfuel = 0;
    nullpat->fuelcons = 0;
    nullpat->speed = 0;
    nullpat->attackdistance = 0;
    nullpat->attackpower = 0;
    nullpat->attackspeed = 0;
    PatList.nullpat = nullpat;
    CCSellData* nullsell = new CCSellData;
    nullsell->id = 0;
    for(UINT i=0;i<50;i++)
        nullsell->item[i] = 0;
    SellList.nullsell = nullsell ;
    for(UINT i=0;i<1000;i++)
    {
        NaturalList.Index[i] = nullnatural;
        PatList.Index[i] = nullpat;
        SellList.Index[i] = nullsell;
    }
    CMap* nullzone = new CMap( );
    nullzone->id =0;
    nullzone->dayperiod = 1;
    nullzone->morningtime = 0;
    nullzone->daytime = 0;
    nullzone->eveningtime = 0;
    nullzone->nighttime = 0;
    nullzone->allowpvp = 0;
    nullzone->allowpat = 0;
    nullzone->MapTime = 0;
    nullzone->LastUpdate = 0;
    nullzone->CurrentTime = 0;
    for(UINT i=0;i<300;i++)
        MapList.Index[i] = nullzone;
    MapList.nullzone = nullzone;
    CUseData* nulluse = new CUseData;
    nulluse->id = 0;
    nulluse->restriction = 0;
    nulluse->type = 0;
    nulluse->price = 0;
    nulluse->pricerate = 0;
    nulluse->weight = 0;
    nulluse->quality = 0;
    nulluse->pricevalue = 0;
    for(int i=0;i<2;i++)
    {
        nulluse->usecondition[i] = 0;
        nulluse->useeffect[i] = 0;
    }
    UseList.nulluse = nulluse;
    CProductData* nullproduct = new CProductData;
    nullproduct->id = 0;
    for(UINT i=0;i<50;i++)
    {
        nullproduct->item[i];
        nullproduct->amount[i];
    }
    ProductList.nullproduct = nullproduct;
    for(UINT i=0;i<2000;i++)
    {
        UseList.Index[i] = nulluse;
        ProductList.Index[i] = nullproduct;
    }
    CJemData* nulljem = new CJemData;
    nulljem->id = 0;
    nulljem->type = 0;
    nulljem->price = 0;
    nulljem->pricerate = 0;
    nulljem->weight = 0;
    nulljem->quality = 0;
    nulljem->material = 0;
    for(int i=0;i<2;i++)
    {
        nulljem->stat1[i] = 0;
        nulljem->stat2[i] = 0;
    }
    JemList.nulljem = nulljem ;
    for(UINT i=0;i<4000;i++)
        JemList.Index[i] = nulljem;
    for(UINT i=0;i<500;i++)
    {
        StatsList[i].stat[0] = 0;
        StatsList[i].stat[1] = 0;
        StatsList[i].value[0] = 0;
        StatsList[i].value[1] = 0;
    }
    MapMutex = PTHREAD_MUTEX_INITIALIZER; //fast mutex
    SQLMutex = PTHREAD_MUTEX_INITIALIZER;
    PlayerMutex = PTHREAD_MUTEX_INITIALIZER;
}

// Destructor
CWorldServer::~CWorldServer( )
{
    // free memory
    for(UINT i=0;i<FairyList.size();i++)
        delete FairyList.at(i);
    for(UINT i=0;i<PartyList.size();i++)
        delete PartyList.at(i);
    for(UINT i=0;i<TeleGateList.size();i++)
        delete TeleGateList.at(i);
    for(UINT i=0;i<SkillList.size();i++)
        delete SkillList.at(i);
    for(UINT i=0;i<MDropList.size();i++)
        delete MDropList.at(i);
    for(UINT i=0;i<NPCData.size();i++)
        delete NPCData.at(i);
    for(UINT i=0;i<JemList.Data.size();i++)
        delete JemList.Data.at(i);
    for(UINT i=0;i<NaturalList.Data.size();i++)
        delete NaturalList.Data.at(i);
    for(UINT i=0;i<PatList.Data.size();i++)
        delete PatList.Data.at(i);
    for(UINT i=0;i<ProductList.Data.size();i++)
        delete ProductList.Data.at(i);
    for(UINT i=0;i<SellList.Data.size();i++)
        delete SellList.Data.at(i);
    for(UINT i=0;i<UseList.Data.size();i++)
        delete UseList.Data.at(i);
    for(UINT i=0;i<MapList.Map.size();i++)
        delete MapList.Map.at(i);
    for(UINT i=0;i<10;i++)
        for(UINT j=0;j<EquipList[i].Data.size();j++)
            delete EquipList[i].Data.at(j);
    for(UINT i=0;i<ClientList.size();i++)
    {
        delete (CPlayer*)ClientList.at(i)->player;
        delete ClientList.at(i);
    }
    delete EquipList[0].nullequip;
    delete JemList.nulljem;
    delete NaturalList.nullnatural;
    delete PatList.nullpat;
    delete ProductList.nullproduct;
    delete SellList.nullsell;
    delete UseList.nulluse;
    delete MapList.nullzone;
    delete DB;
}

// This cleans up our clients mess :P
void CWorldServer::OnClientDisconnect( CClientSocket* thisclient )
{
    if(thisclient->player==NULL) return;
	CPlayer* player = (CPlayer*)thisclient->player;
	if(!player->Session->isLoggedIn) return;
    if(!player->Saved)
    {
    	player->savedata();
        player->Session->isLoggedIn = false;
        //send packet to change messenger status (offline)
    	BEGINPACKET( pak, 0x7e1 );
    	ADDBYTE    ( pak, 0xfa );
    	ADDWORD    ( pak, player->Session->userid );//CharInfo->charid );CharInfo->charid );
    	ADDBYTE    ( pak, 0x00 );
    //	cryptPacket( (char*)&pak, NULL );
    //	send( csock, (char*)&pak, pak.Size, 0 );
        SendISCPacket (&pak);
    }
    if ( player->Fairy ){
        GServer->FairyList.at(player->FairyListIndex)->assigned = false;
        GServer->FairyList.at(player->FairyListIndex)->LastTime = clock();
        GServer->FairyList.at(player->FairyListIndex)->ListIndex = 0;
        GServer->FairyList.at(player->FairyListIndex)->WaitTime = GServer->Config.FairyWait * (rand()% GServer->GetFairyRange(1)+ GServer->GetFairyRange(0));
        player->Fairy = false;
        player->FairyListIndex = 0;
        GServer->DoFairyStuff(player, 0);
        // recalculate FairyMax
        int oldFairyMax = GServer->Config.FairyMax;
        GServer->Config.FairyMax = (int)ceil((float)GServer->ClientList.size() / 50.0); //(1 fairy more every 50 player)
	    if( oldFairyMax > GServer->Config.FairyMax ){
            GServer->FairyList.erase( GServer->FairyList.begin() + GServer->FairyList.size() );
        }
    }
    if(player->Party->party!=NULL)
    {
        CParty* party = player->Party->party;
        BEGINPACKET( pak, 0x7d2 );
        ADDWORD    ( pak, 0xff00 );
        ADDDWORD   ( pak, player->CharInfo->charid );
        bool pflag = false;
        party->RemovePlayer( player );
        if(party->Members.size()>1)
        {
            for(UINT i=0;i<party->Members.size();i++)
            {
                CPlayer* othermember = party->Members.at(i);
                if(!pflag)
                {
                    ADDDWORD( pak, othermember->CharInfo->charid );
                    if(player->Party->IsMaster)
                        othermember->Party->IsMaster = true;
                    pflag = true;
                }
                othermember->client->SendPacket( &pak );
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
                othermember->client->SendPacket( &pak );
                othermember->Party->party = NULL;
                othermember->Party->IsMaster = true;
            }
            RemoveParty( party );
            delete party;
        }
    }
    DB->QExecute("UPDATE accounts SET online=false where id=%u", player->Session->userid );
}

// Server is started, lets run our loop :D
void CWorldServer::ServerLoop( )
{
	fd_set		fds;
	int			activity;
    maxfd = 0;
	sockaddr_in	ClientInfo;
	SOCKET		NewSocket;
	timeval		timeout;
	maxfd = sock;
	OnServerStep();
	do
	{
        if (!Ping( ))
          isActive = false;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;
        NewSocket = INVALID_SOCKET;
        FD_ZERO( &fds );
        pthread_mutex_lock( &PlayerMutex );
		if(!Config.usethreads)
    	   FillFDS( &fds );
		FD_SET( sock, &fds );
		activity = select( maxfd+1, &fds, NULL, NULL, &timeout );
		if ( activity == 0 )
        {
             FD_SET( sckISCII, &fds );
             activity = select( maxfd+1, &fds, NULL, NULL, &timeout );
             if ( activity == 0 )
             {
                    pthread_mutex_unlock( &PlayerMutex );
                	#ifdef _WIN32
                	Sleep(1);
                	#else
                	usleep(1);
                	#endif
                    continue;
             }
            if ( activity < 0 && errno != EINTR )
            {
    			#ifdef _WIN32
    			Log( MSG_ERROR, "Select command failed. Error #%i", WSAGetLastError() );
    			#else
    			Log( MSG_ERROR, "Select command failed. Error #%i", errno );
    			#endif
    			isActive = false;
    		}
    		if ( FD_ISSET( sckISCII, &fds ) )
            {
    			int clientinfolen = sizeof( sockaddr_in );
                #ifdef _WIN32
           		NewSocket = accept( sckISCII, (sockaddr*)&ClientInfo, (int*)&clientinfolen );
           		#else
        		NewSocket = accept( sckISCII, (sockaddr*)&ClientInfo, (socklen_t*)&clientinfolen );
                #endif
    			// TODO: check if server is full
    			if (NewSocket != INVALID_SOCKET)
    				AddUser( NewSocket, &ClientInfo, true);
    			else
    			{
    			    #ifdef _WIN32
    			    Log( MSG_ERROR, "Error accepting socket: %i", WSAGetLastError() );
    			    #else
    			    Log( MSG_ERROR, "Error accepting socket: %i", errno );
    			    #endif
                }
    		}
        }
		if ( activity < 0 && errno != EINTR )
        {
			#ifdef _WIN32
			Log( MSG_ERROR, "Select command failed. Error #%i", WSAGetLastError() );
			#else
			Log( MSG_ERROR, "Select command failed. Error #%i", errno );
			#endif
			isActive = false;
		}
		if ( FD_ISSET( sock, &fds ) )
        {
			int clientinfolen = sizeof( sockaddr_in );
            #ifdef _WIN32
       		NewSocket = accept( sock, (sockaddr*)&ClientInfo, (int*)&clientinfolen );
       		#else
    		NewSocket = accept( sock, (sockaddr*)&ClientInfo, (socklen_t*)&clientinfolen );
            #endif
			// TODO: check if server is full
			if (NewSocket != INVALID_SOCKET)
				AddUser( NewSocket, &ClientInfo, false);
			else
			{
			    #ifdef _WIN32
			    Log( MSG_ERROR, "Error accepting socket: %i", WSAGetLastError() );
			    #else
			    Log( MSG_ERROR, "Error accepting socket: %i", errno );
			    #endif
            }
		}
		if(!Config.usethreads)
    		HandleClients( &fds );
    	pthread_mutex_unlock( &PlayerMutex );
    	#ifdef _WIN32
    	Sleep(1);
    	#else
    	usleep(1);
    	#endif
	} while( isActive );
}

// load the servers encryption
void CWorldServer::LoadEncryption()
{
	GenerateCryptTables( CryptTable, 0x87654321 );
	srand ( (unsigned)time(NULL) );
}

// Return a new clientsocket structure
CClientSocket* CWorldServer::CreateClientSocket( )
{
    //DB->QExecute( "UPDATE channels SET connected=%i where id=%i and type=2", ConnectedClients, Config.ServerID );
    CClientSocket* client = new CClientSocket;
    CPlayer* player = new CPlayer( client );
    client->player = (void*) player;
    return client;
}

// Deletes an old clientsocket structure
void CWorldServer::DeleteClientSocket( CClientSocket* thisclient )
{
    //DB->QExecute( "UPDATE channels SET connected=%i where id=%i and type=2", ConnectedClients, Config.ServerID );
    if(thisclient->player!=NULL)
    {
        CPlayer* player = (CPlayer*) thisclient->player;
        CMap* map = MapList.Index[player->Position->Map];
        pthread_mutex_lock( &MapMutex );
        map->RemovePlayer( player );
        pthread_mutex_unlock( &MapMutex );
    	delete player;
    }
    else delete thisclient;
	Log( MSG_INFO, "User Disconnected" );
}

void CWorldServer::SendISCServerInfo( )
{
	BEGINPACKET( pak, 0x400 );
	ADDBYTE( pak,2 );
	ADDDWORD( pak, Config.ServerID );
	ADDDWORD( pak, Config.ParentID );
	ADDDWORD( pak, Config.WorldPort );
	ADDSTRING( pak, Config.WorldIP );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, Config.ServerName );
	ADDBYTE( pak,0 );
	SendISCPacket( &pak );
}

// This function is called just before the server starts
bool CWorldServer::OnServerReady( )
{
    ServerOnline = true;
    GServer = this;
	clock_t timer = clock();
	LastUpdateTime = clock();

	//SendISCServerInfo( );

    //Load our Server Info
    LoadSTBData( );
    LoadZoneData( );
    LoadConsItem( );
    LoadSellData( );
    LoadProductItem( );
    LoadPatItem( );
    LoadNaturalItem( );
    LoadJemItem( );
    LoadEquip( );
    LoadItemStats( );
    LoadSkillData( );
    LoadStatusData( );

    //LoadDropsData( );
    LoadPYDropsData( );
    LoadSkillBookDropsData( );
    LoadConfig( );

    LoadNPCData( );
    LoadTeleGateData( );
    LoadRespawnData( );
#ifndef USEIFO
    LoadMonsterSpawn( );
#else
    LoadMobGroups( );
#endif
    LoadNPCs( );
    LoadMonsters( );
    LoadUpgrade( );
    LoadQuestData( );
    LoadAipData();
    //LoadZones();
    CleanConnectedList( );
    Log(MSG_INFO, "Database Loaded                      " );
    pthread_create( &WorldThread[WORLD_THREAD], &at, WorldProcess, NULL);
    pthread_create( &WorldThread[VISUALITY_THREAD], &at, VisibilityProcess, NULL);
    pthread_create( &MapThread[0], &at, MapProcess, NULL);
    Log( MSG_INFO, "Process Loaded. WorldDelay %i | MapDelay %i | VisualDelay %i",Config.WorldDelay,Config.MapDelay,Config.VisualDelay);

    DB->QExecute( "DELETE FROM channels WHERE id=%u", Config.ServerID);
    if(!DB->QExecute("INSERT INTO channels (id,name,host,port,connected,maxconnections,owner) VALUES (%i,'%s','%s',%u,0,%i,%i)",
            Config.ServerID, Config.ServerName, Config.WorldIP, Config.WorldPort, Config.MaxConnections, Config.ParentID))
    {
        Log(MSG_WARNING, "Error accessing to database, the other server will not connect to WorldServer" );
    }

   		//say hello to char server
    	BEGINPACKET( pak, 0x500 );
        ADDDWORD   ( pak, Config.CharPass );
        ADDDWORD   ( pak, Config.ServerID );
        ADDWORD    ( pak, Config.WorldsPort );
        SendISCPacket( &pak );
///////////////////////////////////////////////////////////////////////////////////
	float loadtime = (float)( clock() - timer ) / CLOCKS_PER_SEC;
	Log(  MSG_INFO, "Server took %.4f seconds to load", loadtime );

	return true;
}

// Send Levelup To charserver
bool CWorldServer::SendLevelUPtoChar(CPlayer *thisclient)
{

	BEGINPACKET( pak, 0x79e );
	ADDWORD( pak, thisclient->CharInfo->charid );//Session->userid );//CharInfo->charid );
	ADDWORD( pak, thisclient->Stats->Level );
	//cryptPacket( (char*)&pak, NULL );
	//send( csock, (char*)&pak, pak.Size, 0 );
	SendISCPacket( &pak );

	RESETPACKET( pak, 0x79e );
	ADDWORD( pak, thisclient->CharInfo->charid );
	ADDWORD( pak, thisclient->Stats->Level );
	thisclient->client->SendPacket(&pak);
    return true;
}

bool CWorldServer::pakGiveItem( CPlayer* thisclient, unsigned char itemtype, unsigned short itemid, unsigned short count )
{
	char itemtypes[15] = {4,0,0,0,0,0,0,0,0,0,1,2,2,4,3};
	CPacket pakout;
	CItem thisitem;

	thisitem.Clear( );
	thisitem.itemtype = itemtype;
	thisitem.itemnum = itemid;
	thisitem.count = count;
	if( itemtypes[thisitem.itemtype] == 0 ) {
		thisitem.appraised = true;
		thisitem.lifespan = 100;
		thisitem.gem = 0;
		thisitem.durability = STB_ITEM[thisitem.itemtype-1].rows[thisitem.itemnum][29];
	}

	pakout.StartPacket( 0x71f );
	pakout.AddByte( 0 );
	while( thisitem.count > 0 )
	{
		unsigned char thisslot = thisclient->AddItem( thisitem );
		if( thisslot == 0xff ) break;
		pakout.AddByte( thisslot );
		pakout.AddWord( thisclient->items[thisslot].GetPakHeader( ) );
		pakout.AddDWord( thisclient->items[thisslot].GetPakData( ) );
		pakout.Buffer[0]++;
	}
	thisclient->client->SendPacket( &pakout );

	return true;
}

bool CWorldServer::pakGiveExp( CPlayer* thisclient, unsigned exp )
{
	CPacket pakout;
	thisclient->CharInfo->Exp+= exp;
	if(!thisclient->CheckPlayerLevelUP( )){
	pakout.StartPacket( 0x79b );
	pakout.AddDWord( thisclient->CharInfo->Exp );
	pakout.AddWord( thisclient->CharInfo->stamina );
	pakout.AddWord( 0 );
	thisclient->client->SendPacket( &pakout );}
	return true;
}

bool CWorldServer::pakGiveMoney( CPlayer* thisclient, unsigned amount )
{
	CPacket pakout;
	thisclient->CharInfo->Zulies += amount;
	pakout.StartPacket( 0x71e );
	pakout.AddQWord( thisclient->CharInfo->Zulies );
	thisclient->client->SendPacket( &pakout );
	return true;
}

void CWorldServer::DePopulateZone( CPlayer* thisclient, int zonex, int zoney )//compile warning fix by Fluorescent abs -> fabs
{
	CPacket pakout, pakout2;
	if( !thisclient->Session->inGame || fabs( thisclient->Position->current.x - zonex ) > 1 || fabs( thisclient->Position->current.y - zoney ) > 1 )
	{
		pakout.StartPacket( 0x794 );

		for( unsigned i=0; i<ClientList.size( ); i++ ) {
			CPlayer* otherclient = (CPlayer*)ClientList[i];
			if( !otherclient->Session->inGame || thisclient==otherclient ) continue;
		if( otherclient->Position->current.x == zonex && otherclient->Position->current.y == zoney && otherclient->Position->Map == thisclient->Position->Map )
			{
				pakout2.StartPacket( 0x794 );
				pakout2.AddWord( thisclient->clientid );
				otherclient->client->SendPacket( &pakout2 );

				pakout.AddWord( otherclient->clientid );
			}
		}

		for( unsigned i=0; i<MapList.Map.size( ); i++ ) {
             CMap* thismap = MapList.Map.at(i);
                 for(UINT j=0;j<thismap->MonsterSpawnList.size();j++)
                 {
			CMonster* thismonster = thismap->MonsterList.at(j);
			if( thismonster->Position->current.x == zonex && thismonster->Position->current.y == zoney && thismonster->Position->Map == thisclient->Position->Map )
				pakout.AddWord( thismonster->clientid );

		for( unsigned j=0; j<thismap->NPCList.size( ); j++ ) {
			CNPC* thisnpc = thismap->NPCList[j];
			if( thisnpc->pos.x == zonex && thisnpc->pos.y == zoney && thisnpc->posMap == thisclient->Position->Map )
				pakout.AddWord( thisnpc->clientid );
		}

		for( unsigned j=0; j<thismap->DropsList.size( ); j++ ) {
			CDrop* thisdrop = thismap->DropsList[j];
			if( thisdrop->pos.x == zonex && thisdrop->pos.y == zoney && thisdrop->posMap == thisclient->Position->Map )
				pakout.AddWord( thisdrop->clientid );
		}
    }
}

		if( pakout.Size > 6 ) thisclient->client->SendPacket( &pakout );
	}
}

bool CWorldServer::pakTeleport( CPlayer* thisclient, unsigned short map, float x, float y )
{

     fPoint coord;
     coord.x = x/100;
     coord.y = y/100;
     MapList.Index[map]->TeleportPlayer( thisclient, coord, false );
	 return true;
}

// disconect all the clients
void CWorldServer::DisconnectAll()
{
    for(UINT i=0;i<ClientList.size();i++)
    {
        CPlayer* otherclient = (CPlayer*) ClientList.at(i)->player;
		if(otherclient->Session->isLoggedIn)
		{
            otherclient->savedata( );
            otherclient->Saved = true;
            otherclient->client->isActive = false;
        }
	}
}

// Load Server configuration
void CWorldServer::LoadConfigurations( char* file )
{
    //Database
	Config.SQLServer.pcServer   = ConfigGetString ( file, "mysql_host", "localhost" );
	Config.SQLServer.pcDatabase = ConfigGetString ( file, "mysql_database", "roseon_beta" );
    Config.SQLServer.pcUserName = ConfigGetString ( file, "mysql_user", "root" );
	Config.SQLServer.pcPassword = ConfigGetString ( file, "mysql_pass", "" );
	Config.SQLServer.pcPort     = ConfigGetInt    ( file, "mysql_port", 3306 );
    //Server
	Config.ServerID             = ConfigGetInt    ( file, "serverid", 1 );
	Config.ServerType           = 2; // World always = 2
	Config.WorldPort            = ConfigGetInt    ( file, "serverport", 29200 );
	Config.WorldIP              = ConfigGetString ( file, "serverip", "127.0.0.1" );
	Config.ParentID             = ConfigGetInt    ( file, "parentid", 1 );
	Config.ServerName           = ConfigGetString ( file, "servername", "Channel" );
    Config.MaxConnections       = ConfigGetInt    ( file, "maxconnections", 100 );
    Config.CharIP               = ConfigGetString ( file, "charip", "127.0.0.1" );
    Config.CharPort             = ConfigGetInt    ( file, "charport", 29110 );
    Config.WorldsPort           = ConfigGetInt    ( file, "WorldsPort", 29210 );
    //World
    Config.MinimumAccessLevel   = ConfigGetInt    ( file, "minimal_access_level", 100 );
	Config.usethreads           = ConfigGetInt    ( file, "usethreads", 0 )==0?false:true;
    Config.DROP_TYPE            = ConfigGetInt    ( file, "drop_type", 2 );
	Config.WELCOME_MSG          = ConfigGetString ( file, "welcome_msg", "Welcome to Rose Online" );
    Config.AUTOSAVE             = ConfigGetInt    ( file, "autosave", 0 );
	Config.SAVETIME             = ConfigGetInt    ( file, "savetime", 3600 );
    Config.MapDelay             = ConfigGetInt    ( file, "mapdelay", 10 );
    Config.WorldDelay           = ConfigGetInt    ( file, "worlddelay", 200 );
    Config.VisualDelay          = ConfigGetInt    ( file, "visualdelay", 500 );
    Config.Partygap             = ConfigGetInt    ( file, "partygap", 10 );
    Config.MaxStat              = ConfigGetInt    ( file, "maxstat", 254 );
    Config.FairyMode            = ConfigGetInt    ( file, "fairy", 1 );
    Config.FairyStay            = ConfigGetInt    ( file, "fairystay", 20 );
    Config.FairyWait            = ConfigGetInt    ( file, "fairywait", 15 );
    Config.FairyMax             = ConfigGetInt    ( file, "fairymax", 0);
    Config.FairyTestMode        = ConfigGetInt    ( file, "fairytestmode", 1);
    Config.Cfmode               = ConfigGetInt    ( file, "cfmode", 0);

    //Password
	Config.LoginPass            = ConfigGetInt    ( file, "loginpass", 123456 );
	Config.CharPass             = ConfigGetInt    ( file, "charpass", 123456 );
	Config.WorldPass            = ConfigGetInt    ( file, "worldpass", 123456 );
    if(Config.AUTOSAVE==1)
        Log( MSG_INFO, "Autosaving Every %i minutes", Config.SAVETIME/60 );

    LoadCommandLevels();
}

// Load commands from commands.ini [by Paul_T]
void CWorldServer::LoadCommandLevels( void )
{
    Config.Command_Ani = ConfigGetInt    ( "commands.ini", "ani", 299 );
    Config.Command_Ann = ConfigGetInt    ( "commands.ini", "ann", 299 );
    Config.Command_Ban = ConfigGetInt    ( "commands.ini", "ban", 299 );
    Config.Command_Cha = ConfigGetInt    ( "commands.ini", "cha", 299 );
    Config.Command_ChangeFairyWait = ConfigGetInt    ( "commands.ini", "changefairywait", 299 );
    Config.Command_ChangeFairyStay = ConfigGetInt    ( "commands.ini", "changefairystay", 299 );
    Config.Command_ChangeFairyTestMode = ConfigGetInt    ( "commands.ini", "changefairytestmode", 299 );
    Config.Command_Class = ConfigGetInt    ( "commands.ini", "class", 299 );
    Config.Command_Convert = ConfigGetInt    ( "commands.ini", "convert", 299 );
    Config.Command_Cfmode = ConfigGetInt    ( "commands.ini", "cfmode", 299 );
    Config.Command_DelSpawn = ConfigGetInt    ( "commands.ini", "delspawn", 299 );
    Config.Command_DQuest = ConfigGetInt    ( "commands.ini", "dquest", 299 );
    Config.Command_Drop = ConfigGetInt    ( "commands.ini", "drop", 299 );
    Config.Command_DSpawn = ConfigGetInt    ( "commands.ini", "dspawn", 299 );
    Config.Command_ESpawn = ConfigGetInt    ( "commands.ini", "espawn", 299 );
    Config.Command_Exp = ConfigGetInt    ( "commands.ini", "exp", 299 );
    Config.Command_Face = ConfigGetInt    ( "commands.ini", "face", 299 );
    Config.Command_Give2 = ConfigGetInt    ( "commands.ini", "give2", 299 );
    Config.Command_GiveFairy = ConfigGetInt    ( "commands.ini", "givefairy", 299 );
    Config.Command_GiveZuly = ConfigGetInt    ( "commands.ini", "givezuly", 299 );
    Config.Command_Go = ConfigGetInt    ( "commands.ini", "go", 299 );
    Config.Command_Goto = ConfigGetInt    ( "commands.ini", "goto", 299 );
    Config.Command_GoToMap = ConfigGetInt    ( "commands.ini", "gotomap", 299 );
    Config.Command_Hair = ConfigGetInt    ( "commands.ini", "hair", 299 );
    Config.Command_Heal = ConfigGetInt    ( "commands.ini", "heal", 299 );
    Config.Command_Here = ConfigGetInt    ( "commands.ini", "here", 299 );
    Config.Command_Hide = ConfigGetInt    ( "commands.ini", "hide", 299 );
    Config.Command_Info = ConfigGetInt    ( "commands.ini", "info", 299 );
    Config.Command_IQuest = ConfigGetInt    ( "commands.ini", "iquest", 299 );
    Config.Command_Item = ConfigGetInt    ( "commands.ini", "item", 299 );
    Config.Command_Job = ConfigGetInt    ( "commands.ini", "job", 299 );
    Config.Command_Kick = ConfigGetInt    ( "commands.ini", "kick", 299 );
    Config.Command_KillInRange = ConfigGetInt    ( "commands.ini", "killinrange", 299 );
    Config.Command_Level = ConfigGetInt    ( "commands.ini", "level", 299 );
    Config.Command_LevelUp = ConfigGetInt    ( "commands.ini", "levelup", 299 );
    Config.Command_ManageFairy = ConfigGetInt    ( "commands.ini", "managefairy", 299 );
    Config.Command_Mdmg = ConfigGetInt    ( "commands.ini", "mdmg", 299 );
    Config.Command_Mon = ConfigGetInt    ( "commands.ini", "mon", 299 );
    Config.Command_Monster    = ConfigGetInt    ( "commands.ini", "monster", 299 );
    Config.Command_Move = ConfigGetInt    ( "commands.ini", "move", 299 );
    Config.Command_Moveto = ConfigGetInt    ( "commands.ini", "moveto", 299 );
    Config.Command_Mute = ConfigGetInt    ( "commands.ini", "mute", 299 );
    Config.Command_Npc = ConfigGetInt    ( "commands.ini", "npc", 299 );
    Config.Command_Pak = ConfigGetInt    ( "commands.ini", "pak", 299 );
    Config.Command_Pak2 = ConfigGetInt    ( "commands.ini", "pak2", 299 );
    Config.Command_Pakm = ConfigGetInt    ( "commands.ini", "pakm", 299 );
    Config.Command_Partylvl = ConfigGetInt      ( "commands.ini", "partylvl", 299);
    Config.Command_PlayerInfo = ConfigGetInt    ( "commands.ini", "playerinfo", 299 );
    Config.Command_Pdmg = ConfigGetInt    ( "commands.ini", "pdmg", 299 );
    Config.Command_Pvp = ConfigGetInt    ( "commands.ini", "pvp", 299 );
    Config.Command_Rate = ConfigGetInt    ( "commands.ini", "rate", 299 );
    Config.Command_Reload = ConfigGetInt    ( "commands.ini", "reload", 299 );
    Config.Command_ReloadQuest = ConfigGetInt    ( "commands.ini", "reloadquest", 299 );
    Config.Command_Rules = ConfigGetInt    ( "commands.ini", "rules", 99 );
    Config.Command_Save = ConfigGetInt    ( "commands.ini", "save", 299 );
    Config.Command_ServerInfo = ConfigGetInt    ( "commands.ini", "serverinfo", 299 );
    Config.Command_Set = ConfigGetInt    ( "commands.ini", "set", 299 );
    Config.Command_Settime = ConfigGetInt    ( "commands.ini", "settime", 299 );
    Config.Command_ShopType = ConfigGetInt    ( "commands.ini", "shoptype", 299 );
    Config.Command_Shutdown = ConfigGetInt    ( "commands.ini", "shutdown", 299 );
    Config.Command_SSpawn = ConfigGetInt    ( "commands.ini", "sspawn", 299 );
    Config.Command_Stat = ConfigGetInt    ( "commands.ini", "stat", 299 );
    Config.Command_Summon = ConfigGetInt    ( "commands.ini", "summon", 299 );
    Config.Command_TargetInfo = ConfigGetInt    ( "commands.ini", "targetinfo", 299 );
    Config.Command_Tele = ConfigGetInt    ( "commands.ini", "tele", 299 );
    Config.Command_TeleToMe = ConfigGetInt    ( "commands.ini", "teletome", 299 );
    Config.Command_Transx = ConfigGetInt    ( "commands.ini", "transx", 299 );
    Config.Command_Who = ConfigGetInt    ( "commands.ini", "who", 299 );
    Config.Command_Who2 = ConfigGetInt    ( "commands.ini", "who2", 299 );
    Config.Command_Broadcast = ConfigGetInt    ( "commands.ini", "broadcast", 299 );
    Config.Command_GlobalTime  = ConfigGetInt    ( "commands.ini", "globaldelay", 30 );
    Config.Command_GlobalPrefix = ConfigGetString    ( "commands.ini", "globalprefix", "[Broadcast]" );

}

bool CWorldServer::Ping( ) {
  return DB->Ping( );
}

void CWorldServer::ReceivedISCPacket( CPacket* pak )
{
	switch( pak->Command )
	{
	case 0x100:
		break;
	case 0x101:
		SendISCServerInfo( );
		break;
	case 0x300:
		break;
	case 0x301:
		break;
	case 0x400:
		break;
	case 0x401:
		break;
	case 0x500:
		break;
	case 0x501:
		break;
	case 0x502:
	{
		for( unsigned i=0; i<ClientList.size( ); i++ ) {
			CPlayer* otherclient = (CPlayer*)ClientList[i];
			if( otherclient->Session->inGame )
			{
				BEGINPACKET( pakout, 0x702 );
				ADDSTRING( pakout, (char*)&pak->Buffer[0] );
				ADDBYTE( pakout, 0 );
				otherclient->client->SendPacket( &pakout );
			}
		}
		break;
	}
	case 0x503:
		exit( 0 );
		break;
	case 0x580:
		break;
	case 0x581:
		break;
	default:
		Log( MSG_DEBUG, "UNKNOWN ISC PACKET - 0x%x %04x %04x", pak->Command, pak->Size, pak->Unused );
	}
}

// Incoming packet
bool CWorldServer::OnReceivePacket( CClientSocket* thisclient, CPacket *P )
{//Log( MSG_INFO, "(SID:%i) Received packet. Command:%04x Size:%04x", thisclient->sock, P->Command, P->Size );
	switch( P->Command )
	{
        case 0x0500: return pakCSReady          ( (CPlayer*)thisclient->player, P );
        case 0x0502: return pakCharDSClient     ( (CPlayer*)thisclient->player, P );
    	case 0x0505: return pakCSCharSelect     ( (CPlayer*)thisclient->player, P );
    	case 0x0700: return pakPing             ( (CPlayer*)thisclient->player, P );
    	case 0x0707: return pakExit             ( (CPlayer*)thisclient->player, P );
    	case 0x070b: return pakDoIdentify       ( (CPlayer*)thisclient->player, P );
    	case 0x071c: return pakCharSelect       ( (CPlayer*)thisclient->player, P );
    	case 0x0730: return pakQuestTrigger     ( (CPlayer*)thisclient->player, P );
    	case 0x0753: return pakDoID             ( (CPlayer*)thisclient->player, P );
    	case 0x0755: return pakUserDied         ( (CPlayer*)thisclient->player, P );
    	case 0x0756: return pakSaveTown         ( (CPlayer*)thisclient->player, P );
    	case 0x0762: return pakWeight           ( (CPlayer*)thisclient->player, P );
    	case 0x0771: return pakStopChar         ( (CPlayer*)thisclient->player, P );
    	case 0x0781: return pakDoEmote          ( (CPlayer*)thisclient->player, P );
    	case 0x0782: return pakChangeStance     ( (CPlayer*)thisclient->player, P );
    	case 0x0783: return pakNormalChat       ( (CPlayer*)thisclient->player, P );
    	case 0x0784: return pakWhisper          ( (CPlayer*)thisclient->player, P );
    	case 0x0785: return pakShout            ( (CPlayer*)thisclient->player, P );
    	case 0x0786: return pakPartyChat        ( (CPlayer*)thisclient->player, P );
    	case 0x0798: return pakStartAttack      ( (CPlayer*)thisclient->player, P );
      	case 0x079f: return pakShowHeal         ( (CPlayer*)thisclient->player, P );
    	case 0x079a: return pakMoveChar         ( (CPlayer*)thisclient->player, P );
    	case 0x07a1: return pakNPCBuy           ( (CPlayer*)thisclient->player, P );
        case 0x07a3: return pakUseItem          ( (CPlayer*)thisclient->player, P );
        case 0x07a4: return pakDoDrop           ( (CPlayer*)thisclient->player, P );
    	case 0x07a5: return pakChangeEquip      ( (CPlayer*)thisclient->player, P );
    	case 0x07a7: return pakPickDrop         ( (CPlayer*)thisclient->player, P );
    	case 0x07a8: return pakGate             ( (CPlayer*)thisclient->player, P );
    	case 0x07a9: return pakAddStats         ( (CPlayer*)thisclient->player, P );
    	case 0x07aa: return pakMoveSkill        ( (CPlayer*)thisclient->player, P );
    	case 0x07ab: return pakEquipABC         ( (CPlayer*)thisclient->player, P );
    	case 0x07af: return pakCraft            ( (CPlayer*)thisclient->player, P );
        case 0x07b1: return pakLevelUpSkill     ( (CPlayer*)thisclient->player, P );
        case 0x07b2: return pakSkillSelf        ( (CPlayer*)thisclient->player, P );
    	case 0x07b3: return pakStartSkill       ( (CPlayer*)thisclient->player, P );
    	case 0x07b4: return pakSkillAOE         ( (CPlayer*)thisclient->player, P );
    	case 0x07c0: return pakTradeAction      ( (CPlayer*)thisclient->player, P );
    	case 0x07c1: return pakTradeAdd         ( (CPlayer*)thisclient->player, P );
    	case 0x07ad: return pakStorage          ( (CPlayer*)thisclient->player, P );
    	case 0x07ae: return pakChangeStorage    ( (CPlayer*)thisclient->player, P );
    	case 0x07ba: return pakidentify         ( (CPlayer*)thisclient->player, P );
    	case 0x07bc: return pakModifiedItem     ( (CPlayer*)thisclient->player, P );
    	case 0x07bf: return true;//add to wishlist
    	case 0x07c2: return pakOpenShop         ( (CPlayer*)thisclient->player, P );
    	case 0x07c3: return pakCloseShop        ( (CPlayer*)thisclient->player, P );
        case 0x07c4: return pakShowShop         ( (CPlayer*)thisclient->player, P );
        case 0x07c5: return pakBuyShop          ( (CPlayer*)thisclient->player, P );
        case 0x07c6: return pakSellShop         ( (CPlayer*)thisclient->player, P );
    	case 0x07ca: return pakChangeCart       ( (CPlayer*)thisclient->player, P );
    	case 0x07cb: return pakRepairHammer      ( (CPlayer*)thisclient->player, P );
    	case 0x07cd: return pakRepairItem       ( (CPlayer*)thisclient->player, P );
    	case 0x07d0: return pakPartyActions     ( (CPlayer*)thisclient->player, P );
    	case 0x07d1: return pakPartyManager     ( (CPlayer*)thisclient->player, P );
    	case 0x07d7: return pakPartyOption      ( (CPlayer*)thisclient->player, P );
    	case 0x07d8: return pakModifiedItemDone ( (CPlayer*)thisclient->player, P );
        case 0x07d9: return pakItemMall         ( (CPlayer*)thisclient->player, P );
    	case 0x07da: return pakStoreZuly        ( (CPlayer*)thisclient->player, P );
    	case 0x07dd: return pakRideRequest      ( (CPlayer*)thisclient->player, P );
    	case 0x07e0: return pakCreateClan       ( (CPlayer*)thisclient->player, P );
    	case 0x07e1: return pakClanManager      ( (CPlayer*)thisclient->player, P );
    	case 0x07eb: return pakPrintscreen      ( (CPlayer*)thisclient->player, P );
    	case 0x0808: return pakGameGuard        ( (CPlayer*)thisclient->player, P );
    	default:
    		Log( MSG_WARNING, "(SID:%i) Received unknown packet. Command:%04x Size:%04x", thisclient->sock, P->Command, P->Size );
		break;
	}
	return true;
}
// -----------------------------------------------------------------------------------------
