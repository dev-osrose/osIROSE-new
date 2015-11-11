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

 developed with Main erose/hrose source server + some change from the original eich source
 */


#include "loginserver.h"

// Send Server encryption
bool
CLoginServer::pakEncryptionRequest ( CLoginClient* thisclient, CPacket* P )
{
	try
	{
		thisclient->CryptStatus.CurAddValue = rand () | ( rand () << 16 );
		STARTPACKET( pak, 0x7FF, 11 );
		SETBYTE( pak, 0x00, 0x02 );
		SETDWORD( pak, 0x01, thisclient->CryptStatus.CurAddValue );
		thisclient->SendPacket ( &pak );
		return true;
	}
	catch ( ... )
	{
		Log ( MSG_ERROR, "pakEncryptionRequest" );
		return false;
	}
}

// Packet when user login (chck user and pass)
bool
CLoginServer::pakUserLogin ( CLoginClient* thisclient, CPacket* P )
{
	try
	{
		if ( thisclient->isLoggedIn )
			return false;

		thisclient->username.assign (
				(const char*) &P->Buffer, 32,
				( P->Size - 6 - 32 ) > 16 ? 16 : P->Size - 6 - 32 );
		thisclient->password.assign ( (const char*) &P->Buffer, 0, 32 );

		BEGINPACKET( pak, ePacketType::LSV_LOGIN_REQ );

		std::unique_ptr<sql::ResultSet> result =
				DB->QStore (
						"SELECT id,password,accesslevel,online FROM accounts WHERE username='%s'",
						thisclient->username.c_str () );
		if ( result == NULL )
			return false;

		if ( result->next () )
		{
			int res = 0;
#ifdef _WIN32
			res = ( result->getString ( 1 ) != thisclient->password );
#else
			res = ( result->getString ( 1 ) != thisclient->password );
#endif
			if ( res == 0 )
			{
				if ( result->getInt ( 3 ) == 1 )
				{   // characters is already logged
					Log ( MSG_WARNING, "Account %s try re-login",
								thisclient->username.c_str () );
					ADDBYTE( pak, 4 );
					ADDDWORD( pak, 0 );
					thisclient->SendPacket ( &pak );

					DB->QExecute (
							"update accounts set login_count=1 WHERE username='%s'",
							thisclient->username.c_str () );
					return true;
				}
				thisclient->accesslevel = result->getInt ( 2 );
				if ( thisclient->accesslevel < Config.MinimumAccessLevel )
				{ //The server are under inspection
					ADDBYTE( pak, 0 );
					ADDDWORD( pak, 0 );
					thisclient->SendPacket ( &pak );
					return true;
				}
				if ( thisclient->accesslevel > 0 )
				{
					thisclient->userid = result->getInt ( 0 );
					thisclient->isLoggedIn = true;

					//OK!
					ADDDWORD( pak, 0x0c000000 ); //ADDDWORD( pak, 0x6f000000 );
					ADDBYTE( pak, 0 );
					result = DB->QStore ( "SELECT id,name FROM channels WHERE owner=0" );

					if ( result == NULL )
						return false;

					while ( result->next () )
					{
						if ( Config.Testserver )
						{
							ADDBYTE( pak, 63 + result->getInt ( 0 ) );
						}
						else
						{
							ADDBYTE( pak, 48 + result->getInt ( 0 ) );
						}
						ADDSTRING( pak, result->getString ( 1 ).c_str() );
						ADDBYTE( pak, 0 );
						ADDDWORD( pak, result->getInt ( 0 ) );
					}
				}
				else
				{
					//BANNED
					ADDBYTE( pak, 5 );
					ADDDWORD( pak, 0 );
				}
			}
			else
			{
				//BAD PASSWORD
				ADDBYTE( pak, 3 );
				ADDDWORD( pak, 0 );
			}
		}
		else
		{
			if ( Config.CreateLoginAccount )
			{
				if ( !DB->QExecute (
						"INSERT into accounts (username,password,accesslevel) values ('%s','%s',100)",
						thisclient->username.c_str (), thisclient->password.c_str () ) )
					return true;
				Log ( MSG_INFO, "New Account created '%s'",
							thisclient->username.c_str () );
			}
			//BAD USERNAME
			ADDBYTE( pak, 2 );
			ADDDWORD( pak, 0 );
		}
		/*
		 1 - general error   | 4 - your account is already logged
		 6 - topup account   | 7 - cannot connect to server please try again
		 8 - server exceeded | 9 - account have not been verified
		 10 - login failed   | 11 - ip capacity is full
		 */
		thisclient->SendPacket ( &pak );
		return true;
	}
	catch ( ... )
	{
		Log ( MSG_ERROR, "Error in pakUserLogin" );
		return false;
	}

}

// Send server list
bool
CLoginServer::pakGetServers ( CLoginClient* thisclient, CPacket* P )
{
	try
	{
		if ( !thisclient->isLoggedIn )
			return false;

		unsigned servernum = GETDWORD( ( *P ), 0 );
		std::unique_ptr<sql::ResultSet> result = DB->QStore (
				"SELECT id,name,connected,maxconnections FROM channels WHERE owner=%i",
				servernum );

		if ( result == NULL )
			return false;

		BEGINPACKET( pak, 0x704 );
		ADDDWORD( pak, servernum );
		ADDBYTE( pak, (uint8_t )result->rowsCount() ); //old function
		while ( result->next() )
		{
			uint32_t connected = result->getInt( 2 );
			uint32_t maxconnections = result->getInt( 3 );
			uint8_t id = result->getInt( 0 );
			uint8_t status = ( ( 100 * connected )
					/ ( maxconnections == 0 ? 1 : maxconnections ) ) & 0xff;

			ADDWORD( pak, id );
			ADDBYTE( pak, 0 );
			ADDWORD( pak, status );
			ADDSTRING( pak, result->getString( 1 ).c_str() ); // Name
			ADDBYTE( pak, 0 );
		}

		thisclient->SendPacket ( &pak );
		return true;
	}
	catch ( ... )
	{
		Log ( MSG_ERROR, "Error in pakGetServers" );
		return false;
	}
}

// Send server IP
bool
CLoginServer::pakGetIP ( CLoginClient* thisclient, CPacket* P )
{
	try
	{
		if ( !thisclient->isLoggedIn )
			return false;

		uint32_t servernum = GETDWORD( ( *P ), 0 );
		uint8_t channelnum = GETBYTE( ( *P ), 4 );

		BEGINPACKET( pak, 0x70a );

		if ( !DB->QExecute (
				"UPDATE accounts SET lastsvr=%i,lastip='%s' WHERE id=%i", channelnum,
				inet_ntoa ( thisclient->clientinfo.sin_addr ), thisclient->userid ) )
			return false;

		std::unique_ptr<sql::ResultSet> result = DB->QStore (
				"SELECT host,port,connected,maxconnections FROM channels WHERE id=%i",
				servernum );

		if ( result == NULL )
			return false;

		if ( result->next() )
		{
			Log ( MSG_WARNING,
						"Player selected a invalid channel or channel offline" );
			return true;
		}

		uint32_t connected = result->getInt(2);
		uint32_t maxconnections = result->getInt(3);

		ADDBYTE( pak, 0 ); //atoi(row[0]) ); // What is status? It's NULL in tables - Drakia
		ADDDWORD( pak, thisclient->userid );
		ADDDWORD( pak, 0x87654321 );
		ADDSTRING( pak, result->getString(0).c_str() );
		ADDBYTE( pak, 0 );
		ADDWORD( pak, result->getInt(1) );
		thisclient->SendPacket ( &pak );
		return true;
	}
	catch ( ... )
	{
		Log ( MSG_ERROR, "Error in pakGetIP" );
		return false;
	}
}

