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
#include "character.h"

//virtual [called when a enemy hit this character]
bool CCharacter::OnBeAttacked( CCharacter* Enemy )
{
    return true;
}

// virtual [called when this character die]
bool CCharacter::OnDie( )
{
    return true;
}

// virtual [called when enemy die]
bool CCharacter::OnEnemyDie( CCharacter* Enemy )
{
    return true;
}

// virtual [called when a enmy is on sigh]
bool CCharacter::OnEnemyOnSight( CCharacter* Enemy )
{
    return true;
}

// virtual [called when this character spawn]
bool CCharacter::OnSpawn( bool )
{
    return true;
}

// virtual [called when this character is almost dead]
bool CCharacter::OnAlmostDie( )
{
    return true;    
}

// virtual [called when character is far]
bool CCharacter::OnFar( )
{
    return true;
}
