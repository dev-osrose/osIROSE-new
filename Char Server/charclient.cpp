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
#include "charserver.h"

// Constructor
CCharClient::CCharClient( )
{
    userid = 0;
    platinum = false;
    memset( &username, '\0', 17 );
    memset( &password, '\0', 33 );
    memset( &charname, '\0', 17 );
    accesslevel = 0;
    channel = 0;
    clanid = 0;
    clan_rank = 0;
    level = 0;
    job = 0;
    charid = 0;
    isLoggedIn = false;
    logout = false;
}

// Deconstructor
CCharClient::~CCharClient( )
{
    
}
