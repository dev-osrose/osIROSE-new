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
#include "worldmonster.h"

// update attack values and destiny  position
bool CMonster::UpdateValues( )
{     return true;
/*    if(IsSummon( ))
    {
        CPlayer* thisclient = GetOwner( );
        if(thisclient!=NULL)if(thisclient->Stats->HP>0)
        {
            if(!IsOnBattle( ) && thisclient->IsAttacking( ))
            {
                Battle->target = thisclient->Battle->target;
                Battle->atktarget = Battle->target;
                Battle->atktype = NORMAL_ATTACK;
                Battle->contatk = true;
                CCharacter* Enemy = GetCharTarget( );
                if(Enemy!=NULL)
                    StartAction( Enemy, NORMAL_ATTACK );
            }
            else
            if(!IsOnBattle( ))
            {
                Position->source = thisclient->Position->current;
                float distance = GServer->distance( Position->destiny , thisclient->Position->current );
                if((distance>15 && !IsOnBattle()) || distance>20)
                    Move( );                
            }
        }
        else
        {
            UnspawnMonster( );
            return false;
        }
    }*/
/*	if(!IsMoving( ) && !IsOnBattle( ) && CanMove( ))		
    {
        clock_t etime = clock() - Position->lastMoveTime;
        if(etime > 20*CLOCKS_PER_SEC) 
            Move( );
    }*/
    //if(!IsSummon( ))
    //{
        //CPlayer* player = GetNearPlayer( 10 );
        //if(player!=NULL)
            //OnEnemyOnSight( (CCharacter*)player );
    //}
    if(IsMonster( ))
    {}
    return true;
}

// Spawn a monster
void CMonster::SpawnMonster( CPlayer* player, CMonster* thismon )
{
    BEGINPACKET( pak, 0x792 );
	ADDWORD    ( pak, clientid );
	ADDFLOAT   ( pak, Position->current.x*100 );
	ADDFLOAT   ( pak, Position->current.y*100 );
	ADDFLOAT   ( pak, Position->destiny.x*100 );
	ADDFLOAT   ( pak, Position->destiny.y*100 );
	if(Position->current.x==Position->destiny.x&&Position->current.y==Position->destiny.y)
	{
       ADDBYTE    ( pak, 0x00 );
    }
    else
    {
        ADDBYTE   ( pak, 0x01 );
    }
    ADDDWORD   ( pak, 0x00000000 );
    	
	ADDDWORD   ( pak, Stats->HP );	
	if(thismon->owner != player->clientid)
        {
            CMap* map = GServer->MapList.Index[Position->Map];
            if(IsSummon( ) && map->allowpvp!=0) {ADDDWORD( pak, 0x00000064 );} //Hostil
            else if (IsSummon( ) && map->allowpvp==0) {ADDDWORD ( pak, 0x00000000 );}//Friendly
            else {ADDDWORD( pak, 0x00000064 );} //Hostil
        }
    else {ADDDWORD( pak, 0x00000000 );}//Friendly
    ADDDWORD( pak, GServer->BuildBuffs( this ) );
	ADDWORD   ( pak, montype );
	ADDWORD   ( pak, 0x0000 );
	if(IsSummon( ))
    {
        ADDWORD( pak, owner );
        ADDWORD( pak, 0x0000 ); //id del skill (si es summon de skill)
    }
	player->client->SendPacket( &pak );
}

// UnSpawn a Monster
bool CMonster::UnspawnMonster( )
{
    GServer->MapList.Index[Position->Map]->DeleteMonster( this, true );
    return true;
}

// Move a mob (random place)
void CMonster::Move( )
{
    Position->destiny  = GServer->RandInCircle( Position->source, 5 );
    Position->lastMoveTime = clock();  
    ClearBattle( Battle );
    BEGINPACKET( pak, 0x797 );
	ADDWORD    ( pak, clientid );
	ADDWORD    ( pak, 0x0000 );//???	
	ADDWORD    ( pak, Stats->Move_Speed ); //speed
	ADDFLOAT   ( pak, Position->destiny.x*100 );
	ADDFLOAT   ( pak, Position->destiny.y*100 );
	ADDWORD    ( pak, 0xcdcd );
	ADDBYTE    ( pak, thisnpc->stance );
	GServer->SendToVisible(&pak, this);
}

// Move a mob (specified point)
void CMonster::MoveTo( fPoint nPos, bool randcircle )
{                          
    if(randcircle) Position->destiny  = GServer->RandInCircle( nPos, 5 );
    else Position->destiny  = nPos;
    Position->lastMoveTime = clock();
    BEGINPACKET( pak, 0x797 );
	ADDWORD    ( pak, clientid );
	ADDWORD    ( pak, 0x0000 ); // ???
	ADDWORD    ( pak, Stats->Move_Speed );	
	ADDFLOAT   ( pak, Position->destiny.x*100 );
	ADDFLOAT   ( pak, Position->destiny.y*100 );
	ADDWORD    ( pak, 0xcdcd );
	ADDBYTE    ( pak, thisnpc->stance);
	GServer->SendToVisible(&pak, this );
}
