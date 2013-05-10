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
#include "worldserver.h"

//constructor
CCharacter::CCharacter( )
{
    // STATS
    Stats = new STATS;  
    assert(Stats);
    Stats->Level = 0;
    Stats->HP = 0;
    Stats->MP = 0;
    Stats->MaxHP = 0;
    Stats->MaxMP = 0;        
    Stats->Attack_Power = 0;
    Stats->Defense = 0;
    Stats->Critical = 0;
    Stats->Dodge = 0;
    Stats->Accury = 0;
    Stats->Magic_Defense = 0;
    Stats->Move_Speed = 0;
    Stats->Attack_Speed = 0;
    Stats->Attack_Distance = 0;   
    Stats->MaxWeight = 0;
    Stats->MaxSummonGauge = 0;
    Stats->MPReduction = 0;
    Stats->ExtraDamage = 0;
    // POSITION  
    Position = new POSITION;    
    assert(Position);    
    Position->Map = 0;
    Position->lastMoveTime = 0;
    Position->saved = 0;
    Position->respawn = 0;
    //BATLE
    Battle = new BATTLE;
    assert(Battle);
    ClearBattle( Battle );  
    Battle->castTime = 0;  
    //STATUS
    Status = new STATUS;
    assert(Status);
        //Stats up
    Status->Attack_up = 0xff;
    Status->Defense_up = 0xff;
    Status->Magic_Defense_up = 0xff;
    Status->Accury_up = 0xff;
    Status->Critical_up = 0xff;
    Status->Dodge_up = 0xff;
    Status->Haste_up = 0xff; 
    Status->Dash_up = 0xff;
    Status->HP_up = 0xff;
    Status->MP_up = 0xff;
        //Stats down
    Status->Attack_down = 0xff;
    Status->Defense_down = 0xff;
    Status->Magic_Defense_down = 0xff;
    Status->Accury_down = 0xff;
    Status->Critical_down = 0xff;
    Status->Dodge_down = 0xff;
    Status->Haste_down = 0xff; 
    Status->Dash_down = 0xff;
    Status->HP_down = 0xff;
    Status->MP_down = 0xff;
        //Status
    Status->Stuned = 0xff;
    Status->Poisoned = 0xff;
    Status->Muted = 0xff;
    Status->ExtraDamage_up = 0xff;
    Status->ExtraDamage_down = 0xff;
    Status->CanAttack = true;
    Status->CanRun = true;
    Status->CanMove = true;
    Status->CanCastSkill = true;   
        // Stance
    Status->Stance = RUNNING;              
    for(int i=0;i<30;i++)
    {
        MagicStatus[i].Buff = 0;
        MagicStatus[i].Value =0;
        MagicStatus[i].Duration = 0;
        MagicStatus[i].BuffTime = 0;
    }
    CharType = 0;
    clientid = 0xffff;
    damagecounter = 0;
}

//deconstructor
CCharacter::~CCharacter( )
{
    delete Stats;
    delete Position;
    delete Battle;
    delete Status;
}

// Virtual
bool CCharacter::UpdateValues( )
{
    return true;
}

// update position
void CCharacter::UpdatePosition( )
{
    if(IsOnBattle( ) && Battle->target!=0)
    {
        CCharacter* Target = GetCharTarget( );
        if(Target!=NULL)  
        {
            if(IsMonster())
            {
                float distance = GServer->distance( Position->current, Position->source );
                if(distance>50)//AI should take care of this
                {
                    OnFar( );
                }
                //else Position->destiny = Target->Position->current;
            }
            //else Position->destiny = Target->Position->current;
        }
        else ClearBattle( Battle );
    }
    if(!IsMoving()){
      /*  if(IsPlayer())*/Position->lastMoveTime = clock();
        return;
        }
	float dx = Position->destiny.x - Position->current.x;
	float dy = Position->destiny.y - Position->current.y;
	float distance = sqrt( (dx*dx) + (dy*dy) );
    float ntime = ( distance / (float(Stats->Move_Speed)/100)) * CLOCKS_PER_SEC;// * 100000 ); 
    clock_t etime = clock() - Position->lastMoveTime;       	
	if (ntime<=etime || distance==0) 
    {		
		Position->current.x = Position->destiny.x;
		Position->current.y = Position->destiny.y;		
    }
	else 
    {
		Position->current.x = dx*(etime/ntime) + Position->current.x;
		Position->current.y = dy*(etime/ntime) + Position->current.y;
	}
	Position->lastMoveTime = clock();    
}

