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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
   USA.

    developed with Main erose/hrose source server + some change from the
   original eich source
*/
#include "charserver.h"

// Constructor
CCharServer::CCharServer( string fn )
{
	LoadConfigurations( (char*)fn.c_str( ) );
	GServer = this;
}

// Destructor
CCharServer::~CCharServer( )
{
}

// Return a new client socket structure
CCharClient* CCharServer::CreateClientSocket( )
{
	CCharClient* thisclient = new ( nothrow ) CCharClient( );
	return thisclient;
}

// load server encryption
void CCharServer::LoadEncryption( )
{
	GenerateCryptTables( CryptTable, 0x87654321 );
}

// When Server is ready
bool CCharServer::OnServerReady( )
{
	DB->QExecute( "DELETE FROM channels WHERE id=%u", Config.ServerID );
	if ( !DB->QExecute( "INSERT INTO channels "
	                    "(id,name,host,port,connected,maxconnections,owner) "
	                    "VALUES (%i,'%s','%s',%i,0,%i,%i)",
	                    Config.ServerID, Config.CharName, Config.CharIP,
	                    Config.CharPort, Config.MaxConnections,
	                    Config.ParentID ) )
	{
		Log( MSG_WARNING, "Error accessing to database, the other server will "
		                  "not connect to CharServer" );
	}

	// Load all our clans
	std::unique_ptr< sql::ResultSet > result = DB->QStore(
	    "SELECT id,logo,back,name,cp,grade,slogan,news FROM list_clan" );

	if ( result == NULL )
		return false;

	result->beforeFirst( );
	while ( result->next( ) )
	{
		CClans* thisclan = new CClans;
		assert( thisclan ); // maybe remove
		thisclan->id    = result->getInt( "id" );
		thisclan->logo  = result->getInt( "logo" );
		thisclan->back  = result->getInt( "back" );
		thisclan->cp    = result->getInt( "cp" );
		thisclan->grade = result->getInt( "grade" );
		strcpy( thisclan->name, result->getString( "name" ).c_str( ) );
		strcpy( thisclan->slogan, result->getString( "slogan" ).c_str( ) );
		strcpy( thisclan->news, result->getString( "news" ).c_str( ) );
		ClanList.push_back( thisclan );
	}

	// Load Clans
	for ( uint32_t i = 0; i < ClanList.size( ); i++ )
	{
		CClans* Clan = ClanList.at( i );
		result       = DB->QStore( "SELECT id,char_name,clan_rank FROM characters "
		                     "WHERE clanid=%i order by clan_rank",
		                     Clan->id );
		if ( result == NULL )
			return false;

		result->beforeFirst( );
		while ( result->next( ) )
		{
			CClanMembers* newmember = new CClanMembers;
			assert( newmember );
			newmember->id        = result->getInt( "id" );
			newmember->clan_rank = result->getInt( "clan_rank" );
			strcpy( newmember->name, result->getString( "char_name" ).c_str( ) );
			Clan->ClanMembers.push_back( newmember );
		}
	}
	Log( MSG_INFO, "Clans Information Loaded" );
	return true;
}

// When a a client is disconnected
void CCharServer::OnClientDisconnect( CClientSocket* thisclient )
{
	CCharClient* thisclientwc = (CCharClient*)thisclient;
	DB->QExecute( "UPDATE accounts SET online=0 WHERE username='%s'",
	              thisclientwc->username );
	if ( !thisclientwc->logout )
	{
		// Send logout message to friends
		for (uint32_t i = 0; i < thisclientwc->FriendList.size( ); i++)
		{
			CFriendList* Friend = thisclientwc->FriendList.at( i );

			CCharClient* otherclient = GetClientByID( Friend->id );
			if ( otherclient != NULL )
			{
				ChangeMessengerStatus( thisclientwc, otherclient, 0x08 );
			}
		}

		// set logout message to clan
		CClans* thisclan = GetClanByID( thisclientwc->clanid );
		if ( thisclan == NULL )
			return;

		for (uint32_t i = 0; i < thisclan->ClanMembers.size( ); i++)
		{
			CClanMembers* ClanMember  = thisclan->ClanMembers.at( i );
			CCharClient*  otherclient = GetClientByID( ClanMember->id );
			if ( otherclient != NULL )
				ChangeClanStatus( thisclientwc, otherclient, 0xff );
		}
	}
}

// Deletes an old client socket structure
void CCharServer::DeleteClientSocket( CClientSocket* thisclient )
{
	if ( thisclient->isserver )
		Log( MSG_INFO, "Server disconnected" );
	else
		Log( MSG_INFO, "User disconnected" );
	delete (CCharClient*)thisclient;
}

// Load Server configuration
void CCharServer::LoadConfigurations( char* file )
{
	try
	{
		Config.LoadConfig( file );
		//Config.ServerType = 1; // Char always = 1
	}
	catch ( ... )
	{
		Log( MSG_FATALERROR, "Error parsing configuration file" );
	}
}

// Incoming packet
bool CCharServer::OnReceivePacket( CClientSocket* thisclient, CPacket* P )
{
	switch ( P->Header.Command )
	{

	case 0x0500: return pakWSReady( (CCharClient*)thisclient, P );
	case 0x0501: return pakLoginConnected( (CCharClient*)thisclient, P );
	case 0x0502: return pakLoginDSClient( (CCharClient*)thisclient, P );
	case 0x0505: return pakWSCharSelect( (CCharClient*)thisclient, P );
	case 0x070b: return pakDoIdentify( (CCharClient*)thisclient, P );
	case 0x0712: return pakGetCharacters( (CCharClient*)thisclient, P );
	case 0x0713: return pakCreateChar( (CCharClient*)thisclient, P );
	case 0x0714: return pakDeleteChar( (CCharClient*)thisclient, P );
	case 0x0715: return pakRequestWorld( (CCharClient*)thisclient, P );
	case 0x0787: return pakClanChat( (CCharClient*)thisclient, P );
	case 0x07e0: return pakClanManager( (CCharClient*)thisclient, P );
	case 0x07e1: return pakMessengerManager( (CCharClient*)thisclient, P );
	case 0x07e2: return pakMessengerChat( (CCharClient*)thisclient, P );
	case 0x07e3: return pakChatrooms( (CCharClient*)thisclient, P );
	case 0x07e5: return pak7e5( (CCharClient*)thisclient, P );
	case 0x07e6: return pakUploadCM( (CCharClient*)thisclient, P );
	case 0x07e7: return pakDownloadCM( (CCharClient*)thisclient, P );
	case 0x07e8: return pakClanIconTime( (CCharClient*)thisclient, P );
	case 0x079e: return pakUpdateLevel( (CCharClient*)thisclient, P );
	default:
		Log( MSG_WARNING,
		     "(SID:%i) Received unknown packet. Command:%04x Size:%04x",
		     thisclient->sock, P->Header.Command, P->Header.Size );
		break;
	}
	return true;
}

bool CCharServer::Ping( )
{
	return DB->Ping( );
}
