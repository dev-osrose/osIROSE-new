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
#include "sockets.h"

// Constructor
CServerSocket::CServerSocket( )
{
#ifdef USE124
  csum = new char [0x200];
	csumlen = buildChecksum(csum, "TRose.exe");
  if(csumlen < 0)
    std::cout << "Failed to generate checksum, error: " << csumlen << std::endl;
	else
		std::cout << "Generated checksum" << std::endl;
#endif
    ConnectedClients = 0;
}

// Destructor
CServerSocket::~CServerSocket( )
{
    
}

// Start up our server
bool CServerSocket::StartServer( )
{
	//struct sockaddr_in ain;

	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if (sock == INVALID_SOCKET) 
    {
		Log( MSG_FATALERROR, "Could not create a socket" );
		return false;
	}

    int optval = 1;
    if(setsockopt(sock, SOL_SOCKET,SO_KEEPALIVE,(const char*)&optval,sizeof(optval))==-1)
    {
        Log(MSG_ERROR, "setsockopt:SO_KEEPALIVE" );        
    }    
    setsockopt(sock, IPPROTO_TCP,TCP_NODELAY,(const char*)&optval,sizeof(optval));
	ain.sin_family		= AF_INET;
	ain.sin_addr.s_addr	= INADDR_ANY;
	ain.sin_port		= htons( port );
	memset(&(ain.sin_zero), '\0', 8);
	if ( bind( sock, (const sockaddr*)&ain, sizeof( struct sockaddr ) ) ) 
    {
		Log( MSG_FATALERROR, "Could not bind socket" );
		closesocket( sock );
		sock = INVALID_SOCKET;
		return false;
	}
	if ( listen( sock, SOMAXCONN ) == -1 )
    {
		Log( MSG_FATALERROR, "Could not listen on socket" );
		closesocket( sock );
		sock = INVALID_SOCKET;
		return false;
	}
    if (LOG_THISSERVER == LOG_CHARACTER_SERVER)
    {
        //struct sockaddr_in ain;
        sckISC = socket( AF_INET, SOCK_STREAM, 0 );
    	if (sckISC == INVALID_SOCKET) {
    		Log( MSG_ERROR, "Could not create valid ISC socket (WSK2 ERROR: %i)", WSAGetLastError( ) );
    		return false;
   		}
    	int optval = 1;
        if(setsockopt(sckISC, SOL_SOCKET,SO_KEEPALIVE,(const char*)&optval,sizeof(optval))==-1)
        {
            Log(MSG_ERROR, "setsockopt:SO_KEEPALIVE" );        
        }    
        setsockopt(sckISC, IPPROTO_TCP,TCP_NODELAY,(const char*)&optval,sizeof(optval));
	    sain.sin_family		= AF_INET;
	    sain.sin_addr.s_addr	= INADDR_ANY;
	    sain.sin_port		= htons( Config.CharsPort);//29110 );
	    memset(&(sain.sin_zero), '\0', 8);
	    if ( bind( sckISC, (const sockaddr*)&sain, sizeof( struct sockaddr ) ) ) 
        {
		    Log( MSG_FATALERROR, "Could not bind socket" );
            closesocket( sckISC );
		    sckISC = INVALID_SOCKET;
		    return false;
	    }
	    if ( listen( sckISC, SOMAXCONN ) == -1 )
        {
		    Log( MSG_FATALERROR, "Could not listen on socket" );
		    closesocket( sckISC );
		    sckISC = INVALID_SOCKET;
		    return false;
	    }
	    Log( MSG_INFO, "opened ISC poort %i",29110 );
    	
    }
                       
    if (LOG_THISSERVER == LOG_WORLD_SERVER)
    {
        struct sockaddr_in ain;
    	sckISC = socket( AF_INET, SOCK_STREAM, 0 );
    	if (sckISC == INVALID_SOCKET) {
    		Log( MSG_ERROR, "Could not create valid ISC socket (WSK2 ERROR: %i)", WSAGetLastError( ) );
    		return false;
    	}
    	ain.sin_family		= AF_INET;
    	ain.sin_addr.s_addr	= inet_addr(Config.CharIP);//( "127.0.0.1" );
    	ain.sin_port		= htons( Config.CharPort);//29110 );
    	if ( connect( sckISC, (const sockaddr*)&ain, sizeof( ain ) ) == SOCKET_ERROR ) {
    		Log( MSG_ERROR, "Could not connect to ISC (WSK2 ERROR: %i)", WSAGetLastError( ) );
    		closesocket( sckISC );
    		sckISC = INVALID_SOCKET;
    		return false;
    	}
    	
    	
   	    sckISCII = socket( AF_INET, SOCK_STREAM, 0 );
    	if (sckISCII == INVALID_SOCKET) {
    		Log( MSG_ERROR, "Could not create valid ISC socket (WSK2 ERROR: %i)", WSAGetLastError( ) );
    		return false;
   		}
    	int optval = 1;
        if(setsockopt(sckISCII, SOL_SOCKET,SO_KEEPALIVE,(const char*)&optval,sizeof(optval))==-1)
        {
            Log(MSG_ERROR, "setsockopt:SO_KEEPALIVE" );        
        }    
        setsockopt(sckISCII, IPPROTO_TCP,TCP_NODELAY,(const char*)&optval,sizeof(optval));
	    sain.sin_family		= AF_INET;
	    sain.sin_addr.s_addr	= INADDR_ANY;
	    sain.sin_port		= htons( Config.WorldsPort );//29210 );
	    memset(&(sain.sin_zero), '\0', 8);
	    if ( bind( sckISCII, (const sockaddr*)&sain, sizeof( struct sockaddr ) ) ) 
        {
		    Log( MSG_FATALERROR, "Could not bind socket" );
            closesocket( sckISCII );
		    sckISCII = INVALID_SOCKET;
		    return false;
	    }
	    if ( listen( sckISCII, SOMAXCONN ) == -1 )
        {
		    Log( MSG_FATALERROR, "Could not listen on socket" );
		    closesocket( sckISCII );
		    sckISCII = INVALID_SOCKET;
		    return false;
	    }
	    Log( MSG_INFO, "opened ISC poort %i",Config.WorldsPort );//29210 );
    }
    if (LOG_THISSERVER == LOG_LOGIN_SERVER)
    {
        struct sockaddr_in ain;
    	sckISC = socket( AF_INET, SOCK_STREAM, 0 );
    	if (sckISC == INVALID_SOCKET) {
    		Log( MSG_ERROR, "Could not create valid ISC socket (WSK2 ERROR: %i)", WSAGetLastError( ) );
    		return false;
    	}
     }
	isActive = true;

	if ( !this->OnServerReady( ) )
    {
		Log( MSG_FATALERROR, "Server could not start" );
		closesocket( sock );
		sock = INVALID_SOCKET;
		isActive = false;
		return false;
	}
	Log( MSG_INFO, "Server started on port %i and is listening.", port );
	//ISCThread( );
	ServerLoop( );
	// Nothing past here is ever really called
	OnServerDie( );
	closesocket( sock );

	return true;
}

// Raven0123
void CServerSocket::CryptISCPak( char* pak )
{
	return;
	unsigned paksize = *((unsigned short*)&pak[0]);
	for( unsigned short i = 2; i < paksize; i ++ )
		pak[i] ^= 0x81 * i * paksize;
}

// Raven0123
//void CServerSocket::ISCThread()
bool CClientSocket::ISCThread()
{
    unsigned char buffer[0x400];
	unsigned recvd = 0;
	unsigned bufsize = 0;
	unsigned readlen = 6;
	bool go = true;

	do {

		recvd = recv( sock, (char*)&buffer[bufsize], readlen-bufsize, 0 );
		if( recvd == 0){
			return false;
		}

		if( recvd == SOCKET_ERROR ) {
			return false;
		}

		bufsize += recvd;
		if( bufsize != readlen ) continue;//return;//continue;
		if( bufsize == 6 ) {
			readlen = *((unsigned short*)&buffer[0]);
			if( readlen < 6 ) Log( MSG_ERROR, "Invalid server Packet Header" );
			if( readlen > 6 ) continue;//return;//continue;
		}

		GS->OnReceivePacket( this, (CPacket*)&buffer[0] );
		go = false;

		bufsize = 0;
		readlen = 6;
	} while ( go==true );
	return true;
}

// Server is started, lets run our loop :D
void CServerSocket::ServerLoop( )
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
		if(!Config.usethreads)
    	   FillFDS( &fds );    	
		FD_SET( sock, &fds );
		activity = select( maxfd+1, &fds, NULL, NULL, &timeout );
		if ( activity == 0 )
        {
             // continue;
             FD_ZERO( &fds );
             if(!Config.usethreads)
    	        FillFDS( &fds );
             FD_SET( sckISC, &fds );
             activity = select( maxfd+1, &fds, NULL, NULL, &timeout );
             if ( activity == 0 )continue;
             if ( activity < 0 && errno != EINTR ) 
        {
			#ifdef _WIN32
			Log( MSG_ERROR, "Select command failed. Error #%i", WSAGetLastError() );
			#else
			Log( MSG_ERROR, "Select command failed. Error #%i", errno );			
			#endif
			isActive = false;
		}
		if ( FD_ISSET( sckISC, &fds ) )
        {
			int clientinfolen = sizeof( sockaddr_in );	
            #ifdef _WIN32
       		NewSocket = accept( sckISC, (sockaddr*)&ClientInfo, (int*)&clientinfolen );
       		#else
    		NewSocket = accept( sckISC, (sockaddr*)&ClientInfo, (socklen_t*)&clientinfolen );
            #endif			
			// TODO: check if server is full
			if (NewSocket != INVALID_SOCKET)
            {
				AddUser( NewSocket, &ClientInfo, true );
				
            }
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
				AddUser( NewSocket, &ClientInfo, false );
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
	} while( isActive );
}

// Fills out an FDS for the server
void CServerSocket::FillFDS( fd_set* fds )
{
    for(UINT i=0;i<ClientList.size( );i++)
	{		
        CClientSocket* client = ClientList.at( i );
        if(client->isActive)
        {
		      FD_SET( (unsigned)client->sock, fds );
		      if(client->sock>maxfd)
		          maxfd=client->sock;          		
        }
        else
        {
            DisconnectClient( client );                                       
        }
	}    	
}

// Handle all our clients
void CServerSocket::HandleClients( fd_set* fds )
{
    for(UINT i=0;i<ClientList.size( );i++)
	{
        CClientSocket* client = ClientList.at( i );
        if(!client->isActive)
            continue;
		if(FD_ISSET( client->sock, fds ))
		{
            if (client->isserver == true)
            {
               //Log( MSG_INFO,"ISC PACKET");
               if(!client->ISCThread()){
               client->isActive = false;
               DisconnectClient( client );}
            }
            else
			if(!client->ReceiveData( ) )
			{
                client->isActive = false;
                DisconnectClient( client ); 
			}
		}
	}  
}

// Add a new user to our server
void CServerSocket::AddUser( SOCKET sock, sockaddr_in* ClientInfo, bool server )
{
    ConnectedClients++;
	CClientSocket* thisclient = this->CreateClientSocket( );
	if (thisclient==NULL) 
    { 
		closesocket( thisclient->sock );
		if (thisclient!=0) delete thisclient;
		thisclient=0;
		return;
	}
#ifdef USE124
	thisclient->ct = new char[TABLE_SIZE];
	buildCryptTable(thisclient->ct, csum, csumlen);
#endif
	thisclient->CryptTable = CryptTable;
	thisclient->CryptStatus.CurAddValue = 0;
	thisclient->CryptStatus.CurEncryptionValue = CryptTable->EncryptionStartValue;
	thisclient->GS = this;
	thisclient->sock = sock;
	thisclient->isActive = true;
	
	if (!OnClientConnect( thisclient )) {
		closesocket( thisclient->sock );
		if (thisclient!=0) delete thisclient;
		thisclient=0;
		return;
	}
	thisclient->ClientIP = "";
    thisclient->ClientIP = inet_ntoa( ClientInfo->sin_addr );
    char *tmp;
    memset(&thisclient->ClientSubNet, '\0', 12 );
    sprintf(thisclient->ClientSubNet, "%i.%i.%i",
           (ClientInfo->sin_addr.s_addr )&0xFF, (ClientInfo->sin_addr.s_addr>>8 )&0xFF,
           (ClientInfo->sin_addr.s_addr>>16)&0xFF);	
	ClientList.push_back( thisclient );
	if(Config.usethreads)
	{
        pthread_create( &threads[sock], NULL, ClientMainThread, (PVOID)thisclient);     	   
    }
    memcpy( &thisclient->clientinfo, ClientInfo, sizeof(struct sockaddr_in));
    if(server==true)
    {
        thisclient->isserver=true;
        Log( MSG_INFO, "Server connected from %s", inet_ntoa( ClientInfo->sin_addr ) );
    }
    else
    {
        thisclient->isserver=false;
        Log( MSG_INFO, "User connected from %s", inet_ntoa( ClientInfo->sin_addr ) );
    }
}

// Disconnect our user
void CServerSocket::DisconnectClient( CClientSocket* thisclient )
{
    ConnectedClients--;
	OnClientDisconnect( thisclient );
	closesocket( thisclient->sock );
	thisclient->isActive = false;
  	thisclient->sock = INVALID_SOCKET;
    for(UINT i=0;i<ClientList.size( );i++)
    {
        CClientSocket* client = ClientList.at( i );
        if( client == thisclient )
        {
            ClientList.erase( ClientList.begin( ) + i );
            break;
        }
    }		
	DeleteClientSocket( thisclient );
}

// This function creates an appropriate client socket
CClientSocket* CServerSocket::CreateClientSocket ( )
{
	CClientSocket* thisclient;
    thisclient = new (nothrow) CClientSocket( );	
	return thisclient;
}

// This function deletes an old client socket
void CServerSocket::DeleteClientSocket( CClientSocket* thisclient )
{
     if (thisclient->isserver)
     Log( MSG_INFO, "Server disconnected" );
     else
     Log( MSG_INFO, "User disconnected" );
	delete thisclient;
}

//This function loads the encryption
void CServerSocket::LoadEncryption( )
{
	GenerateCryptTables( CryptTable, 0x87654321 );
	//port = ConfigGetInt("server.conf", "loginport", 29000);
}

// This function is called just before proccessing clients
void CServerSocket::OnServerStep( )
{
}

// This function is called just before the server starts
bool CServerSocket::OnServerReady( )
{
	return true;
}

// This function is called just before the server dies
void CServerSocket::OnServerDie( )
{
	// DOESNT WORK - DAMN CONSOLE APPS
}

// This function is called, if a client receives data
bool CServerSocket::OnReceivePacket( CClientSocket* thisclient, CPacket *P )
{
	return true;
}

// This function is called, if a client connects
bool CServerSocket::OnClientConnect( CClientSocket* thisclient )
{
	return true;
}

// This function is called, if a client disconnects
void CServerSocket::OnClientDisconnect( CClientSocket* thisclient )
{
}

// Raven0123
void CServerSocket::SendISCPacket( CPacket* pak )
{
	send( sckISC, (char*)pak, pak->Size, 0 );
}

void CServerSocket::ReceivedISCPacket( CPacket* pak )
{
	Log( MSG_DEBUG, "GOT ISC PACKET (BASESERVER) - 0x%04x %04x", pak->Command, pak->Size );
}

bool CServerSocket::DoSQL(char *Format, ...) {
	int retval;
	char Buffer[0x1500];
	va_list ap; va_start( ap, Format );
	vsprintf( Buffer, Format, ap ); 
	va_end  ( ap );
 	retval = mysql_query( mysql, Buffer );
	if( retval != 0 ) Log( MSG_ERROR, "MySQL Query Error '%s'", mysql_error( mysql ) );
	return (retval==0);
}

bool CServerSocket::Ping( )
{
  return true;
}
