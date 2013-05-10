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

unsigned char LOG_THISSERVER = 0;
class CLoginServer* GServer;

// Main Funtion
int main(int argc, char *argv[]) 
{
    	LOG_THISSERVER = LOG_LOGIN_SERVER;
    InitWinSocket( ); 
    string fileconf = "loginserver.conf";   
    if(argc>1)
    {
        fileconf = argv[1];
    }
    
	CLoginServer *server = new (nothrow) CLoginServer( fileconf );
	
    MYSQL mysql;
	server->DB = new CDatabase( server->Config.SQLServer.pcServer,
	                    server->Config.SQLServer.pcUserName,
	                    server->Config.SQLServer.pcPassword,
	                    server->Config.SQLServer.pcDatabase,
	                    server->Config.SQLServer.pcPort, &mysql );
    server->port = server->Config.LoginPort;    
	if(server->Config.usethreads)
	   Log( MSG_INFO, "Using Threads" );	  
	// Start server
	if(server->DB->Connect( )==0)  Log( MSG_INFO, "Connected to MySQL server" );
	else
	{
        delete server;
        #ifdef _WIN32
		system("pause");
		#endif
		return -1;        
    }
    server->LoadEncryption ();
    server->StartServer( );
    server->DB->Disconnect( );	
	// Close server
	delete server;
	CloseWinSocket( );
	return EXIT_SUCCESS;
}
