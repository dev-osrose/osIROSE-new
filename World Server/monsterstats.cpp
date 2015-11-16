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

// Get Mob Attack Power
unsigned int CMonster::GetAttackPower( )
{
	unsigned int attack;
	attack = thisnpc->atkpower;
    if(thisnpc->weapon!=0)
        attack += GServer->EquipList[8].Index[thisnpc->weapon]->attackpower;
    if(Status->Attack_up!=0xff)
        attack += MagicStatus[Status->Attack_up].Value;
    if(Status->Attack_down!=0xff)
        attack -= MagicStatus[Status->Attack_down].Value;
	return attack;
}

// Get Monster Defense
unsigned int CMonster::GetDefense( )
{
    unsigned int defense;
    defense =  thisnpc->defense;
    if(thisnpc->subweapon!=0)
        defense += GServer->EquipList[SUBWEAPON].Index[thisnpc->subweapon]->defense;
    if(Status->Defense_up!=0xff)
        defense += MagicStatus[Status->Defense_up].Value;
    if(Status->Defense_down!=0xff)
        defense -= MagicStatus[Status->Defense_down].Value;
    return defense;
}

// Get Monster atk speed
unsigned int CMonster::GetAttackSpeed( )
{
    UINT aspeed = (UINT)thisnpc->atkspeed;
    if(aspeed == 0)
    {
        aspeed = 1; // prevent a /0 when hunting dragon egg
    }
    if(Status->Haste_up != 0xff)
    {
        aspeed += MagicStatus[Status->Haste_up].Value;
    }
    if(Status->Haste_down != 0xff)
    {
        aspeed -= MagicStatus[Status->Haste_down].Value;
    }
    return aspeed;
}

// Get Monster Movement speed
unsigned int CMonster::GetMoveSpeed( )
{
    unsigned int mspeed = 0;
    if (thisnpc->stance==mRUNNING)
    {
       mspeed=thisnpc->rspeed;
    }
    else
    {
        mspeed=thisnpc->wspeed;
    }
    if(Status->Dash_up!=0xff)
        mspeed += MagicStatus[Status->Dash_up].Value;
    if(Status->Dash_down!=0xff)
        mspeed -= MagicStatus[Status->Dash_down].Value;
    return mspeed;
}

// return dodge
unsigned int CMonster::GetDodge( )
{
    UINT dodge = 0;
    dodge = thisnpc->dodge;
    if(dodge == 0) dodge = thisnpc->defense; // prevent a /0 for hawk skill
    if(Status->Dodge_up!=0xff)
        dodge += MagicStatus[Status->Dodge_up].Value;
    if(Status->Dodge_down!=0xff)
        dodge -= MagicStatus[Status->Dodge_down].Value;
    return dodge;
}

// return accury
unsigned int CMonster::GetAccury( )
{
    UINT hitrate = 0;
    hitrate = thisnpc->hitrate;
    if(Status->Accury_up!=0xff)
        hitrate += MagicStatus[Status->Accury_up].Value;
    if(Status->Accury_down!=0xff)
        hitrate -= MagicStatus[Status->Accury_down].Value;
    return hitrate;
}

// return magic defense
unsigned int CMonster::GetMagicDefense( )
{
    UINT mdef = 0;
    mdef = thisnpc->magicdefense;
    if(Status->Magic_Defense_up!=0xff)
        mdef += MagicStatus[Status->Magic_Defense_up].Value;
    if(Status->Magic_Defense_down!=0xff)
        mdef -= MagicStatus[Status->Magic_Defense_down].Value;
    return mdef;
}

// return magic attack
unsigned int CMonster::GetMagicAttack( )
{
    return thisnpc->magicattack;
}


// return max hp
unsigned int CMonster::GetMaxHP( )
{
    UINT MaxHP = 0;
    MaxHP = thisnpc->hp;
    if(owner==0)
        MaxHP *= thisnpc->level;
    if(MaxHP==0)
        MaxHP = thisnpc->shp;
    return MaxHP;
}

unsigned int CMonster::GetCritical( )
{
    return 60;  //60 = 20% of 300 our critical probability
}

float CMonster::GetAttackDistance( )
{
    return thisnpc->atkdistance;
}


// set monster stats values
bool CMonster::SetStats()
{
    if(thisnpc == NULL)
    {
        return false;
    }
    Stats->Level = thisnpc->level;
    Stats->Attack_Power = GetAttackPower( );
    Stats->Defense = GetDefense( );
    Stats->Attack_Speed = GetAttackSpeed( );
    Stats->Move_Speed = GetMoveSpeed( );
    Stats->Dodge = GetDodge( );
    Stats->Accury = GetAccury( );
    Stats->Critical = GetCritical( );
    Stats->Magic_Defense = GetMagicDefense( );
    Stats->magicattack = GetMagicAttack( );
    Stats->Attack_Distance = GetAttackDistance( );
    //Stats->MaxHP = GetMaxHP( );
    //Stats->HP = Stats->MaxHP;
    //Stats->MaxMP = GetMaxMP( );
    //Stats->MP = Stats->MaxMP;
    return true;
}
