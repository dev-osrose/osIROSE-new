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
#define REVISION 32
#include <fstream>
#include "worldserver.h"

#ifdef _WIN32
#pragma comment( lib, "ws2_32.lib" )
#undef close
#endif

CWorldServer* GServer;

unsigned char LOG_THISSERVER;

// Main server function
int main( int argc, char* argv[] )
{
	srand( (unsigned)time( NULL ) );
	srand( rand( ) * time( NULL ) );
	LOG_THISSERVER = LOG_WORLD_SERVER;
	InitWinSocket( );
	string fileconf = "./server.ini";
	if ( argc > 1 )
	{
		fileconf = argv[ 1 ];

		if ( argc > 2 )
		{
			CROSEServerConfig temp;
			temp.LoadConfig( (char*)fileconf.c_str( ), true );
		}
	}

	std::basic_fstream< char > clFileOp( fileconf, std::ios::in );
	if ( clFileOp.is_open( ) == false )
	{
		clFileOp.clear( );
		clFileOp.open( fileconf, std::ios::out ); // Create file
		clFileOp.close( );

		CROSEServerConfig temp;
		temp.LoadConfig( (char*)fileconf.c_str( ), true );
	}

	CWorldServer* server = new ( nothrow ) CWorldServer( fileconf );
	if ( server == NULL )
		return -1;

	// Connect mysql
	server->DB = new CDatabase( server->Config.SQLServer.pcServer,
	                            server->Config.SQLServer.pcUserName,
	                            server->Config.SQLServer.pcPassword,
	                            server->Config.SQLServer.pcDatabase,
	                            server->Config.SQLServer.pcPort );

	if ( server->Config.usethreads )
		Log( MSG_INFO, "Using Threads" );

	// Start server
	server->port = server->Config.WorldPort;

	if ( server->DB->Connect( ) == 0 )
		Log( MSG_INFO, "Connected to MySQL server" );
	else
	{
		delete server;
#ifdef _WIN32
		system( "pause" );
#endif
		return -1;
	}

	server->LoadEncryption( );

	server->StartServer( );
	// Close server
	server->DB->Disconnect( );
	CloseWinSocket( );
	Log( MSG_INFO, "Cleaning memory, please wait..." );
	delete server;
	return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------------------
