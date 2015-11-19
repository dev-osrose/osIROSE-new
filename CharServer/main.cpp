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
#include <fstream>
#include "charserver.h"
#undef close

unsigned char      LOG_THISSERVER = 0;
class CCharServer* GServer;

// Main function
int main( int argc, char* argv[] )
{
	LOG_THISSERVER = LOG_CHARACTER_SERVER;
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
	CCharServer* server = new ( nothrow ) CCharServer( fileconf );

	if ( server == nullptr )
		return -1;

	server->DB = new CDatabase( server->Config.SQLServer.pcServer,
	                            server->Config.SQLServer.pcUserName,
	                            server->Config.SQLServer.pcPassword,
	                            server->Config.SQLServer.pcDatabase,
	                            server->Config.SQLServer.pcPort );

	server->port = server->Config.CharPort;

	if ( server->Config.usethreads )
		Log( MSG_INFO, "Using Threads" );

	if ( server->DB->Connect( ) == 0 )
	{
		Log( MSG_INFO, "Connected to MySQL server" );
	}
	else
	{
		delete server;
#ifdef _WIN32
		system( "pause" );
#endif
		return -1;
	}

	server->LoadEncryption( );

	// Start server
	server->StartServer( );

	// Close server
	server->DB->Disconnect( );
	delete server;
	CloseWinSocket( );
	return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------------------
