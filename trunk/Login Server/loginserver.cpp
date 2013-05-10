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
    LoadConfigurations( (char*)filename.c_str() );    
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
    	CLoginClient* thisclient = new (nothrow) CLoginClient( );
        if(thisclient==NULL)
            return NULL;
    	thisclient->GS = this;
    	return thisclient;
     }
     catch(...)
     {
         Log(MSG_ERROR, "Error in CreateClientSocket");
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
     catch(...)
     {
        Log(MSG_ERROR, "Error in DeleteClientSocket");
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
        //Database
    	Config.SQLServer.pcServer   = ConfigGetString ( file, "mysql_host", "localhost" );
    	Config.SQLServer.pcDatabase = ConfigGetString ( file, "mysql_database", "irose2" );
        Config.SQLServer.pcUserName = ConfigGetString ( file, "mysql_user", "root" );
    	Config.SQLServer.pcPassword = ConfigGetString ( file, "mysql_pass", "root" );
    	Config.SQLServer.pcPort     = ConfigGetInt    ( file, "mysql_port", 3306 );
    	//Server
        Config.ServerID             = ConfigGetInt    ( file, "serverid", 0 );
        Config.ServerType           = 0; // Login is always 0
        Config.LoginIP              = ConfigGetString ( file, "serverip", "127.0.0.1" );
    	Config.LoginPort            = ConfigGetInt    ( file, "serverport", 29000 );
        Config.ServerName           = ConfigGetString ( file, "servername", "Loginserver" );
    	//Passwords
    	Config.LoginPass            = ConfigGetInt    ( file, "loginpass", 123456 );
    	Config.CharPass             = ConfigGetInt    ( file, "charpass", 123456 );	
    	Config.WorldPass            = ConfigGetInt    ( file, "worldpass", 123456 );	
        //Login
        Config.MinimumAccessLevel   = ConfigGetInt    ( file, "accesslevel", 100 );    	
    	Config.usethreads           = ConfigGetInt    ( file, "usethreads", 0 )==0?false:true;
        Config.CreateLoginAccount   = ConfigGetInt    ( file, "CreateLoginAccount", 0 )==0?false:true;
        Config.Testserver           = ConfigGetInt    ( file, "Testserver", 1 )==0?false:true;
    }
    catch(...)
    {
        Log(MSG_FATALERROR, "Error parsing configuration file");
    }
    
}

// Handle packets
bool CLoginServer::OnReceivePacket( CClientSocket* thisclient, CPacket *P )
{
	switch( P->Command )
	{
	           
	   case 0x703: return pakEncryptionRequest( (CLoginClient*)thisclient, P );
	   case 0x708: return pakUserLogin        ( (CLoginClient*)thisclient, P );
	   case 0x704: return pakGetServers       ( (CLoginClient*)thisclient, P ); 
	   case 0x70a: return pakGetIP            ( (CLoginClient*)thisclient, P );
	   case 0x808: return true;
    	default:
    		Log( MSG_WARNING, "[%i]Login Server Received unknown packet. Command:%04x Size:%04x", thisclient->sock, P->Command, P->Size );
		break;
	}
	return true;
}

bool CLoginServer::Ping( ) {
  return DB->Ping( );
}
