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

 developed with Main erose/hrose source server + some change from the original
 eich source
 */

#include "loginserver.h"
#include <ctime>

#ifdef _WIN32
#pragma comment(lib, "Winmm.lib")
#endif

// Send Server encryption
bool CLoginServer::pakEncryptionRequest( CLoginClient* thisclient, CPacket* P )
{
	(void)P;
	try
	{
		STARTPACKET( pak, 0x7FF, 0x23 );
		SETBYTE( pak, 0x00, 0x02 );
		SETDWORD( pak, 0x01, static_cast<dword>(std::time(nullptr)) );
		thisclient->SendPacket( &pak );
		return true;
	}
	catch ( ... )
	{
		Log( MSG_ERROR, "pakEncryptionRequest" );
		return false;
	}
}

// Packet when user login (check user and pass)
bool CLoginServer::pakUserLogin( CLoginClient* thisclient, CPacket* P )
{
	try
	{
		if ( thisclient->isLoggedIn )
			return false;

		thisclient->username.assign(
		    (const char*)&P->Buffer, 32,
		    ( P->Header.Size - 6 - 32 ) > 16 ? 16 : P->Header.Size - 6 - 32 );
		thisclient->password.assign( (const char*)&P->Buffer, 0, 32 );

		CPacket pak( ePacketType::PAKCS_LOGIN_REQ, sizeof( pakLoginReply ) );

		sql::PreparedStatement* prep = DB->QPrepare( "SELECT id, password, accesslevel, `online` FROM accounts WHERE username = ?" );
		prep->setString( 1, thisclient->username );

		std::unique_ptr< sql::ResultSet > result( prep->executeQuery( ) );
		result->beforeFirst();

		if ( result == NULL ) {
			Log(MSG_ERROR, "SELECT query failed");
			return false;
		}

		if ( result->next( ) )
		{
			int res = 0;
#ifdef _WIN32
			res = ( result->getString( 2 ) != thisclient->password );
#else
			res = ( result->getString( 2 ) != thisclient->password );
#endif
			if ( res == 0 )
			{
				if ( result->getInt( 4 ) == 1 )
				{ // characters is already logged
					Log( MSG_WARNING, "Account %s try re-login",
					     thisclient->username.c_str( ) );

					pak.pLoginReply.Result = 4;
					pak.pLoginReply.Right = 0;
					pak.pLoginReply.Unknown = 0;
					thisclient->SendPacket( &pak );

					sql::PreparedStatement* prep =
					    DB->QPrepare( "update accounts set login_count=1 WHERE "
					                  "username=?" );
					prep->setString( 1, thisclient->username );
					prep->execute( );
					DB->QPrepareFree( );

					return true;
				}
				
				thisclient->accesslevel = result->getInt( 3 );
				if ( thisclient->accesslevel < Config.MinimumAccessLevel )
				{ // The server are under inspection
					pak.pLoginReply.Result = 1;
					pak.pLoginReply.Right = 0;
					pak.pLoginReply.Unknown = 0;
					thisclient->SendPacket( &pak );
					return true;
				}

				if ( thisclient->accesslevel > 0 )
				{
					thisclient->userid     = result->getInt( 1 );
					thisclient->isLoggedIn = true;

					// OK!
					pak.pLoginReply.Result = 0;
					pak.pLoginReply.Right = 0;
					pak.pLoginReply.Unknown = 0;
					result = DB->QStore(
					    "SELECT id,name FROM channels WHERE owner=0" );

					if ( result->rowsCount( ) == 0 ) {
						Log(MSG_ERROR, "No Channel found");
						pak.pLoginReply.Result = 1;
						thisclient->SendPacket(&pak);
						return false;
					}

					result->beforeFirst( );
					while ( result->next( ) )
					{
// 						if (Config.Testserver)
// 						{
// 							pak.Add< uint8_t >( 63 + result->getInt( 1 ) );
// 						}
// 						else
// 						{
// 							pak.Add< uint8_t >( 48 + result->getInt( 1 ) );
//  						}
						pak.AddString( result->getString( 2 ).c_str( ), true );
						pak.Add< uint32_t >( result->getInt( 1 ) );
					}
				}
				else
				{
					// BANNED
					pak.pLoginReply.Result = 5;
					pak.pLoginReply.Right = 0;
					pak.pLoginReply.Unknown = 0;
				}
			}
			else
			{
				// BAD PASSWORD
				pak.pLoginReply.Result = 3;
				pak.pLoginReply.Right = 0;
				pak.pLoginReply.Unknown = 0;
			}
		}
		else
		{
			if ( Config.CreateLoginAccount )
			{
				sql::PreparedStatement* prep = DB->QPrepare(
				    "INSERT into accounts (username,password,accesslevel) "
				    "values (?,?,?)" );
				prep->setString( 1, thisclient->username );
				prep->setString( 2, thisclient->password );
				prep->setInt( 3, 100 );

				if ( !prep->execute( ) )
				{
					Log(MSG_ERROR, "Error while writing to the DB");
					DB->QPrepareFree( );
					return true;
				}
				DB->QPrepareFree( );
				Log( MSG_INFO, "New Account created '%s'",
				     thisclient->username.c_str( ) );
			}
			// BAD USERNAME
			pak.pLoginReply.Result = 2;
			pak.pLoginReply.Right = 0;
			pak.pLoginReply.Unknown = 0;
		}
		/*
		 1 - general error   | 4 - your account is already logged
		 6 - top up account   | 7 - cannot connect to server please try again
		 8 - server exceeded | 9 - account have not been verified
		 10 - login failed   | 11 - ip capacity is full
		 */
		thisclient->SendPacket( &pak );
		return true;
	}
	catch ( ... )
	{
		Log( MSG_ERROR, "Error in pakUserLogin" );
		return false;
	}
}

// Send server list
bool CLoginServer::pakGetServers( CLoginClient* thisclient, CPacket* P )
{
	try
	{
		if ( !thisclient->isLoggedIn )
			return false;

		unsigned servernum = GETDWORD( ( *P ), 0 );

		sql::PreparedStatement* prep =
		    DB->QPrepare( "SELECT id,name,connected,maxconnections FROM "
		                  "channels WHERE owner=?" );
		prep->setInt( 1, servernum );
		std::unique_ptr< sql::ResultSet > result( prep->executeQuery( ) );
		DB->QPrepareFree( );

		if ( result == NULL )
			return false;

		CPacket pak( ePacketType::PAKLC_CHANNEL_LIST_REPLY );
		pak.Add< uint32_t >( servernum );
		pak.Add< uint8_t >( (uint8_t)result->rowsCount( ) );
		while ( result->next( ) )
		{
			uint32_t connected      = result->getInt( 3 );
			uint32_t maxconnections = result->getInt( 4 );
			uint8_t  id             = result->getInt( 1 );
			uint8_t  status         = ( ( 100 * connected ) /
			                   ( maxconnections == 0 ? 1 : maxconnections ) ) &
			                 0xff;
			
			pak.Add< uint16_t >( id );
			pak.Add< uint8_t >( 0 );
			pak.Add< uint16_t >( status );
			pak.AddString( result->getString( 2 ).c_str( ), true );
		}

		thisclient->SendPacket( &pak );
		return true;
	}
	catch ( ... )
	{
		Log( MSG_ERROR, "Error in pakGetServers" );
		return false;
	}
}

// Send server IP
bool CLoginServer::pakGetIP( CLoginClient* thisclient, CPacket* P )
{
	try
	{
		if ( !thisclient->isLoggedIn )
			return false;

		uint32_t servernum = GETDWORD( ( *P ), 0 );
		uint8_t channelnum = GETBYTE( ( *P ), 4 );

		CPacket pak( ePacketType::PAKCS_SRV_SELECT_REQ );

		sql::PreparedStatement* prep = DB->QPrepare(
		    "UPDATE accounts SET lastsvr=?,lastip='?' WHERE id=?" );
		prep->setInt( 1, channelnum );
		prep->setString( 2, inet_ntoa( thisclient->clientinfo.sin_addr ) );
		prep->setInt( 3, thisclient->userid );
		bool bRes = prep->execute( );
		DB->QPrepareFree( );

		if ( bRes == false )
			return false;

		prep = DB->QPrepare( "SELECT host,port,connected,maxconnections FROM "
		                     "channels WHERE id=?" );
		prep->setInt( 1, servernum );
		std::unique_ptr< sql::ResultSet > result( prep->executeQuery( ) );
		DB->QPrepareFree( );

		if ( result == NULL )
			return false;

		if ( result->next( ) )
		{
			Log( MSG_WARNING,
			     "Player selected a invalid channel or channel offline" );
			return true;
		}

		// uint32_t connected = result->getInt(3);
		// uint32_t maxconnections = result->getInt(4);

		ADDBYTE( pak, 0 ); // atoi(row[0]) ); // What is status? It's NULL in
		                   // tables - Drakia
		ADDDWORD( pak, thisclient->userid );
		ADDDWORD( pak, 0x87654321 );
		ADDSTRING( pak, result->getString( 1 ).c_str( ) );
		ADDBYTE( pak, 0 );
		ADDWORD( pak, result->getInt( 2 ) );
		thisclient->SendPacket( &pak );
		return true;
	}
	catch ( ... )
	{
		Log( MSG_ERROR, "Error in pakGetIP" );
		return false;
	}
}
