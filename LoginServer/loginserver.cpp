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
#include "loginserver.h"

// Constructor
CLoginServer::CLoginServer( string fn )
{
	filename = fn;
	LoadConfigurations( (char*)filename.c_str( ) );
	GServer = this;
}

// Destructor
CLoginServer::~CLoginServer( )
{
}

// Create client socket (alloc memory)
CLoginClient* CLoginServer::CreateClientSocket( )
{
	try
	{
		CLoginClient* thisclient = new ( nothrow ) CLoginClient( );

		if ( thisclient == NULL )
			return NULL;

		thisclient->GS = this;
		return thisclient;
	}
	catch ( ... )
	{
		Log( MSG_ERROR, "Error in CreateClientSocket" );
		return NULL;
	}
}

//loadserver encryption
void CLoginServer::LoadEncryption( )
{
	GenerateLoginServerCryptTables( CryptTable );
}

// Delete client socket (free memory)
void CLoginServer::DeleteClientSocket( CClientSocket* thisclient )
{
	try
	{
		Log( MSG_INFO, "User disconnected" );
		delete (CLoginClient*)thisclient;
	}
	catch ( ... )
	{
		Log( MSG_ERROR, "Error in DeleteClientSocket" );
	}
}

// Load information when server is ready
bool CLoginServer::OnServerReady( )
{
	return true;
}

// Load Server configuration
void CLoginServer::LoadConfigurations( char* file )
{
	try
	{
		Config.LoadConfig( file );
		//Config.ServerType = 0; // Login is always 0
	}
	catch ( ... )
	{
		Log( MSG_FATALERROR, "Error parsing configuration file" );
	}
}

// Handle packets
bool CLoginServer::OnReceivePacket( CClientSocket* thisclient, CPacket* P )
{
	//TODO:: Change these magic numbers into an enum so we what wtf is going on -Raven
	switch ( P->Header.Command )
	{
	case 0x703: 
		return pakEncryptionRequest( (CLoginClient*)thisclient, P );
	case 0x708:
		return pakUserLogin( (CLoginClient*)thisclient, P );
	case 0x704:
		return pakGetServers( (CLoginClient*)thisclient, P );
	case 0x70a:
		return pakGetIP( (CLoginClient*)thisclient, P );
	case 0x808:
		return true;
	default:
		Log( MSG_WARNING, "[%i]Login Server Received unknown packet. Command:%04x Size:%04x", thisclient->sock, P->Header.Command, P->Header.Size );
		break;
	}
	return true;
}

bool CLoginServer::Ping( )
{
	return DB->Ping( );
}
