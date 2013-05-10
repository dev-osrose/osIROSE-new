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
#include "sockets.h"

// Constructor
CBaseSocket::CBaseSocket( )
{
	sock = INVALID_SOCKET;
	isActive = false;
}

// Destructor
CBaseSocket::~CBaseSocket( )
{
}

// Close all open sockets
void CBaseSocket::CloseSocket( void )
{
	// Leave function, if socket isn't active
	if ( !isActive ) return;	
	// Close socket
	close( sock );
	sock	= INVALID_SOCKET;
	isActive	= false;
}

// Init winsocket, if we are using Windows
bool InitWinSocket ( void )
{
#ifdef _WIN32
	WSADATA wsa;
	return (WSAStartup(MAKEWORD(2,0),&wsa)==0);
#else
	return true;
#endif
}

// Close winsocket, if we are using Windows
void CloseWinSocket( void )
{
#ifdef _WIN32
	WSACleanup( );
#endif
}



