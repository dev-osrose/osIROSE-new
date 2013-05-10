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
bool CLoginServer::pakEncryptionRequest( CLoginClient* thisclient, CPacket* P )
{
     try
     {
    	thisclient->CryptStatus.CurAddValue = rand( ) | (rand( )<<16);
     	STARTPACKET( pak, 0x7FF, 11 );
    	SETBYTE    ( pak, 0x00, 0x02 );
    	SETDWORD   ( pak, 0x01, thisclient->CryptStatus.CurAddValue );
    	thisclient->SendPacket( &pak );
        return true;
     }
    catch(...)
    {
        Log(MSG_ERROR, "pakEncryptionRequest");
    }
}

// Packet when user login (chck user and pass)
bool CLoginServer::pakUserLogin( CLoginClient* thisclient, CPacket* P )
{
     try
     {
    	if ( thisclient->isLoggedIn ) return false;
    	MYSQL_ROW row;
    	thisclient->username.assign( (const char*)&P->Buffer, 32, (P->Size-6-32)>16?16:P->Size-6-32 );
    	thisclient->password.assign( (const char*)&P->Buffer, 0, 32 );
    	BEGINPACKET( pak, 0x708 );
    	MYSQL_RES *result = DB->QStore( "SELECT id,password,accesslevel,online FROM accounts WHERE username='%s'", thisclient->username.c_str() );
    	if(result==NULL) return false;
    	if( mysql_num_rows( result ) == 1 ) 
        {
    		row = mysql_fetch_row(result);
    		int res = 0;
    		#ifdef _WIN32
    		    res = _stricmp( row[1], thisclient->password.c_str() );
    		#else
    		    res = strcasecmp( row[1], thisclient->password.c_str() );
    		#endif		
    		if ( res == 0 ) 
            {
                if(atoi(row[3])==1)
                {   // characters is already logged
                    Log(MSG_WARNING, "Account %s try re-login", thisclient->username.c_str() );
            		ADDBYTE( pak, 4 );
                	ADDDWORD( pak, 0 );    	
                	thisclient->SendPacket( &pak );
            		DB->QFree( );
            		DB->QExecute( "update accounts set login_count=1 WHERE username='%s'", thisclient->username.c_str());
            		  return true;
                }
    			thisclient->accesslevel = atoi(row[2]);
    			if( thisclient->accesslevel < Config.MinimumAccessLevel )
    			{ //The server are under inspection
                    ADDBYTE( pak, 0 );
                    ADDDWORD( pak, 0 );
                    thisclient->SendPacket( &pak );
                    DB->QFree( );
                    return true;
                }
    			if ( thisclient->accesslevel > 0 ) 
                {
    				thisclient->userid = atoi(row[0]);
    				thisclient->isLoggedIn = true;
    				DB->QFree( );
    				//OK!
    				ADDDWORD( pak, 0x0c000000 );//ADDDWORD( pak, 0x6f000000 );
    				ADDBYTE( pak, 0 );
    				result = DB->QStore( "SELECT id,name FROM channels WHERE owner=0" );
                    if(result==NULL) return false;
    				while( row = mysql_fetch_row(result) ) 
                    {
                        if (Config.Testserver){ADDBYTE( pak, 63 + atoi( row[0] ) );}
    					else{
                        ADDBYTE( pak, 48 + atoi( row[0] ) );}
    					ADDSTRING( pak, row[1] );
    					ADDBYTE( pak, 0 );
    					ADDDWORD(pak, atoi( row[0] ) );
    				}
    				DB->QFree( );
    			}else{
    				//BANNED
    				ADDBYTE( pak, 5 );
    				ADDDWORD( pak, 0 );
    				DB->QFree( );
    			}
    		}else{
    			//BAD PASSWORD
    			ADDBYTE( pak, 3 );
    			ADDDWORD( pak, 0 );
    			DB->QFree( );
    		}
    	}
        else
        {
            DB->QFree( );
            if(Config.CreateLoginAccount)
            {
                if(!DB->QExecute("INSERT into accounts (username,password,accesslevel) values ('%s','%s',100)",thisclient->username.c_str(),thisclient->password.c_str()))                            
                   return true;
                Log( MSG_INFO, "New Account created '%s'", thisclient->username.c_str() );       
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
    catch(...)
    {
        Log(MSG_ERROR, "Error in pakUserLogin");
    }
              
}

// Send server list
bool CLoginServer::pakGetServers( CLoginClient* thisclient, CPacket* P )
{
     try
     {
    	if( !thisclient->isLoggedIn ) return false;	
    	MYSQL_ROW row;
    	unsigned servernum = GETDWORD( (*P), 0 );
    	MYSQL_RES *result = DB->QStore( "SELECT id,name,connected,maxconnections FROM channels WHERE owner=%i", servernum );
    	if(result==NULL) return false;
    	BEGINPACKET( pak, 0x704 );
    	ADDDWORD   ( pak, servernum );
    	ADDBYTE    ( pak, (BYTE)mysql_num_rows( result ) ); //old function
    	while(row = mysql_fetch_row(result)) 
        {
        	UINT connected = atoi(row[2]);
        	UINT maxconnections = atoi(row[3]);
        	BYTE id = atoi( row[0] );
        	BYTE status = ((100 * connected) / (maxconnections == 0 ? 1 : maxconnections)) & 0xff;

    		ADDWORD( pak, id );
    		ADDBYTE( pak, 0 );
    		ADDWORD( pak, status );
    		ADDSTRING( pak, row[1] ); // Name
    		ADDBYTE( pak, 0 );
    	}
    	DB->QFree( );
    	thisclient->SendPacket ( &pak );
        return true;
    }
    catch(...)
    {
        Log(MSG_ERROR, "Error in pakGetServers");
    }
}

// Send server IP
bool CLoginServer::pakGetIP( CLoginClient* thisclient, CPacket* P )
{
     try
     {
    	if (!thisclient->isLoggedIn) return false;
    	
    	MYSQL_ROW row;

    	DWORD servernum = GETDWORD( (*P), 0 );
    	BYTE channelnum = GETBYTE( (*P), 4 );

    
    	BEGINPACKET( pak, 0x70a );
    
    	if(!DB->QExecute( "UPDATE accounts SET lastsvr=%i,lastip='%s' WHERE id=%i", channelnum, inet_ntoa( thisclient->clientinfo.sin_addr ), thisclient->userid))
    	   return false;
    	MYSQL_RES *result = DB->QStore( "SELECT host,port,connected,maxconnections FROM channels WHERE id=%i", servernum );
        if(result==NULL) return false;	
    	if(mysql_num_rows(result)!=1)
    	{
            Log(MSG_WARNING, "Player selected a invalid channel or channel offline" );
            DB->QFree( );
            return true;
        }
    	row = mysql_fetch_row(result);
    	UINT connected = atoi(row[2]);
    	UINT maxconnections = atoi(row[3]);
    	
    	ADDBYTE( pak, 0 ); //atoi(row[0]) ); // What is status? It's NULL in tables - Drakia
    	ADDDWORD( pak, thisclient->userid );
    	ADDDWORD( pak, 0x87654321 );
    	ADDSTRING( pak, row[0] );
    	ADDBYTE( pak, 0 );
    	ADDWORD( pak, atoi(row[1]) );
    	DB->QFree( );
    	thisclient->SendPacket ( &pak );
        return true;
    }
    catch(...)
    {
        Log(MSG_ERROR, "Error in pakGetIP");
    }
}

