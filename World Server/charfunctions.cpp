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

    developed with Main erose/hrose source server + some change from the original eich source
*/
#include "character.h"
#include "worldserver.h"

// return true if this character is monster
bool CCharacter::IsMonster( )
{
    if(CharType==2 || CharType==3)
        return true;
    return false;
}

// return true if this character is player
bool CCharacter::IsPlayer( )
{
    if(CharType==1)
        return true;
    return false;
}

// return true if HP is lower to 1
bool CCharacter::IsDead( )
{
    if(Stats->HP<=0)
        return true;
    return false;
}

// return true if this character is summon
bool CCharacter::IsSummon( )
{
    if(CharType==3)
        return true;
    return false;
}

// Virtual function [return item drop]
CDrop* CCharacter::GetDrop( )
{
    return NULL;
}

// start action [attack]
void CCharacter::StartAction( CCharacter* Target, BYTE action, UINT skillid)
{
    BEGINPACKET( pak, 0 );
    switch(action)
    {
        case NORMAL_ATTACK:
        {//Log( MSG_INFO, "case NORMAL_ATTACK");
            RESETPACKET( pak, 0x798 );
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, Target->clientid );
            ADDWORD    ( pak, Stats->Move_Speed );
            ADDFLOAT   ( pak, Target->Position->current.x*100 );
            ADDFLOAT   ( pak, Target->Position->current.y*100 );
            Battle->target = Target->clientid;
            Battle->atktarget = Target->clientid;
            Battle->atktype = action;
            Position->destiny  = Target->Position->current;
            Position->lastMoveTime = clock();
        }
        break;
        case SKILL_ATTACK: //Log( MSG_INFO, "case SKILL_ATTACK");
        case SKILL_BUFF:
        {//Log( MSG_INFO, "case SKILL_BUFF");
        //Log( MSG_INFO, "clientid :%i Target id :%i skillid :%i", clientid, Target->clientid, skillid);
        	RESETPACKET( pak, 0x7b3 );
        	ADDWORD    ( pak, clientid );
        	ADDWORD    ( pak, Target->clientid );
        	ADDWORD    ( pak, skillid );
        	if (clientid==Target->clientid){
        	   ADDWORD( pak, 0x0000 );
            }
        	else { ADDWORD (pak, Stats->Move_Speed);}
            ADDFLOAT   ( pak, Target->Position->current.x*100 );
            ADDFLOAT   ( pak, Target->Position->current.y*100 );
            if (CharType==2&&action==SKILL_ATTACK)
            ADDBYTE    ( pak, 0x08);//8 monster skill attack nMotion ?
            Battle->target = Target->clientid;

            if(action==SKILL_ATTACK) Battle->skilltarget = Target->clientid;
            else Battle->bufftarget = Target->clientid;

            Battle->atktype = action;
            Position->destiny  = Target->Position->current;
            Battle->skillid = skillid;
            Position->lastMoveTime = clock();
        }
        break;
        case SKILL_AOE://Log( MSG_INFO, "case SKILL_AOE");
        case BUFF_SELF://Log( MSG_INFO, "case BUFF_SELF");
        case BUFF_AOE:
        {//Log( MSG_INFO, "case BUFF_AOE");
            RESETPACKET( pak, 0x7b2);
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, skillid );
            Battle->atktype = action;
            Battle->skillid = skillid;
        }
        break;
        case AOE_TARGET:
        {//Log( MSG_INFO, "case AOE_TARGET");
            RESETPACKET( pak, 0x7b4 );
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, skillid );
            ADDFLOAT   ( pak, Position->skilltargetpos.x*100 );
            ADDFLOAT   ( pak, Position->skilltargetpos.y*100 );
            Battle->atktype = action;
            Battle->skillid = skillid;
            Battle->skilltarget = 0;
        }
        break;
        default: return;
    }
    GServer->SendToVisible( &pak, this );
    Battle->contatk = true;
}

bool CCharacter::IsOnBattle( )
{
    if(Battle->atktype==0) return false;
    switch(Battle->atktype)
    {
        case NORMAL_ATTACK:
        {
            if(Battle->atktarget!=0) return true;
        }
        break;
        case SKILL_ATTACK:
        case AOE_TARGET:
        {
            if(Battle->skilltarget!=0 && Battle->skillid!=0) return true;
        }
        break;
        case SKILL_BUFF:
        {
            if(Battle->bufftarget!=0 && Battle->skillid!=0) return true;
        }
        break;
        case SKILL_AOE:
        case SKILL_SELF:
        case BUFF_SELF:
        case BUFF_AOE:
        {
            if(Battle->skillid!=0) return true;
        }
        break;
        default: return false;
    }
}

bool CCharacter::CanAttack( )
{
    clock_t etime = clock() - Battle->lastAtkTime;
    if (Stats->Attack_Speed<1)
    {
        Log( MSG_INFO, "weapon with no attackspeed ");//%i"items[7].itemnum );
        return false;
    }
    if( etime < (CLOCKS_PER_SEC * 100 / Stats->Attack_Speed) ) return false;
    return true;
}

// return true if target is reached
bool CCharacter::IsTargetReached( CCharacter* Enemy, CSkills* skill )
{
     UpdatePosition();
     Enemy->UpdatePosition();
    //??CMap* map = GServer->MapList.Index[Position->Map];
    float fightdistance = GServer->distance( Position->current, Enemy->Position->current );
   // if (!IsPlayer())Log(MSG_INFO,"dist 1 %f",fightdistance);
   // if (!IsPlayer())Log(MSG_INFO,"dist 2 %f",Stats->Attack_Distance);
    if(skill==NULL)
    {
        if((fightdistance)<=Stats->Attack_Distance)
        {
            Position->destiny=Position->current;
            return true;

        }
        Position->destiny=Enemy->Position->current;
    }
    else
    {
        if((fightdistance)<=skill->range)
        {
            Position->destiny=Position->current;
            return true;

        }
        Position->destiny=Enemy->Position->current;
    }
    return false;
}

// return true if the character is moving
bool CCharacter::IsMoving( )
{
    if(Position->current.x == Position->destiny.x && Position->current.y == Position->destiny.y )
        return false;
    return true;
}

// VIRTUAL [add player damage]
void CCharacter::AddDamage( CCharacter* enemy, long int hitpower)
{

}

// return character target
CCharacter* CCharacter::GetCharTarget( )
{
    return GServer->MapList.Index[Position->Map]->GetCharInMap( Battle->target );
}

// VIRTUAL [reduce ammon/bullets/cannons]
void CCharacter::ReduceABC( )
{

}

// VIRTUAL [get extra buff value]
int CCharacter::GetEVBuff( )
{
    return 0;
}

// VIRTUAL [return intelligence]
unsigned int CCharacter::GetInt( )
{
    return 15;
}


// Check For Debuffs
void CCharacter::RefreshBuff( )
{
    bool bflag = false;
    for( UINT i=0;i<30;i++)
    {
        if(MagicStatus[i].Buff==0) continue;
        clock_t etime = clock() - MagicStatus[i].BuffTime;
        if( etime >= MagicStatus[i].Duration * CLOCKS_PER_SEC )
        {
            switch(MagicStatus[i].Buff)
            {
                case A_ATTACK:
                    if(i<15)
                        Status->Attack_up = 0xff;
                    else
                        Status->Attack_down = 0xff;
                    Stats->Attack_Power = GetAttackPower( );
                break;
                case A_DEFENSE:
                    if(i<15)
                        Status->Defense_up= 0xff;
                    else
                        Status->Defense_down = 0xff;
                    Stats->Defense = GetDefense( );
                break;
                case A_ACCUR:
                    if(i<15)
                        Status->Accury_up= 0xff;
                    else
                        Status->Accury_down = 0xff;
                    Stats->Accury = GetAccury( );
                break;
                case A_MRESIST:
                    if(i<15)
                        Status->Magic_Defense_up = 0xff;
                    else
                        Status->Magic_Defense_down = 0xff;
                    Stats->Magic_Defense = GetMagicDefense( );
                break;
                case A_DODGE:
                    if(i<15)
                        Status->Dodge_up = 0xff;
                    else
                        Status->Dodge_down = 0xff;
                    Stats->Dodge = GetDodge( );
                break;
                case A_DASH:
                    if(i<15)
                        Status->Dash_up = 0xff;
                    else
                        Status->Dash_down = 0xff;
                    Stats->Move_Speed = GetMoveSpeed( );
                break;
                case A_HASTE:
                    if(i < 15)
                    {
                        Status->Haste_up = 0xff;
                    }
                    else
                    {
                        Status->Haste_down = 0xff;
                    }
                    Stats->Attack_Speed = GetAttackSpeed();
                break;
                case A_CRITICAL:
                    if(i<15)
                        Status->Critical_up = 0xff;
                    else
                        Status->Critical_down = 0xff;
                    Stats->Critical = GetCritical( );
                break;
                case A_MAX_HP:
                    if(i<15)
                        Status->HP_up = 0xff;
                    else
                        Status->HP_down = 0xff;
                    Stats->MaxHP = GetMaxHP( );
                break;
                case A_MAX_MP:
                    if(i<15)
                        Status->MP_up = 0xff;
                    else
                        Status->MP_down = 0xff;
                    Stats->MaxMP = GetMaxMP( );
                break;
                case A_STUN:
                     Status->Stuned = 0xff;
                     printf("removing stun\n");
                     //StartAction2(NULL, 0, 0, true);
                break;
                case A_POISON:
                     Status->Poisoned = 0xff;
                     printf("removing poison\n");
                break;
                case A_MUTE:
                     Status->Muted = 0xff;
                break;
                case A_GMExtra_Damage:
                case A_Extra_Damage:
                     if(i<15)
                     {
                        Status->ExtraDamage_up = 0xff;
                        Stats->ExtraDamage = 0;
                     }

                     else
                         Status->ExtraDamage_down = 0xff;

                break;
            }
            MagicStatus[i].Buff = 0;
            MagicStatus[i].BuffTime = 0;
            MagicStatus[i].Duration = 0;
            MagicStatus[i].Value = 0;
            bflag = true;
        }
        else if (MagicStatus[i].Buff == A_POISON && etime > 1*CLOCKS_PER_SEC) //Do poison dmg every 1.5 seconds
        {
             Stats->HP -= MagicStatus[i].Value;
             MagicStatus[i].BuffTime+= 1*CLOCKS_PER_SEC;
             MagicStatus[i].Duration-=1;
             printf("did %i poison dmg to the player, still %i seconds and %i HP remain \n", MagicStatus[i].Value, MagicStatus[i].Duration, Stats->HP);

             //A bunch of messy code to send dmg packet
             BEGINPACKET( pak, 0x7b6 );
             ADDWORD    ( pak, clientid );
             ADDWORD    ( pak, 0 );
             ADDDWORD   ( pak, 0x000007f8 );
             ADDBYTE    ( pak, 0x00 );
             ADDDWORD   ( pak, MagicStatus[i].Value );

             //If Enemy is killed
             if( IsDead())
             {
                 printf("char died\n");
                 CDrop* thisdrop = NULL;
                 //ADDDWORD   ( pak, 16 );//i don't think this one belongs in irose
                 if( !IsSummon( ) && !IsPlayer( ))
                 {
                     thisdrop = GetDrop( );
                     if( thisdrop!=NULL)
                     {
                         ADDFLOAT   ( pak, thisdrop->pos.x*100 );
                         ADDFLOAT   ( pak, thisdrop->pos.y*100 );
                         if( thisdrop->type==1)
                         {
                             ADDDWORD( pak, 0xccdf );//0xccccccdf );
                             ADDDWORD( pak, thisdrop->amount );
                         }
                         else
                         {
                             ADDDWORD   ( pak, GServer->BuildItemHead( thisdrop->item ) );
                             ADDDWORD   ( pak, GServer->BuildItemData( thisdrop->item ) );
                         }
                         ADDWORD    ( pak, thisdrop->clientid );
                         ADDWORD    ( pak, thisdrop->owner );
                         CMap* map = GServer->MapList.Index[thisdrop->posMap];
                         map->AddDrop( thisdrop );
                     }
                 }
                 GServer->SendToVisible( &pak, this, thisdrop );
             }

             //If enemy is still alive
             else
             {
                 ADDDWORD   ( pak, 4 );
                 GServer->SendToVisible( &pak, this );
             }
         }
    }
    if(bflag)
    {
        BEGINPACKET( pak,0x7b7 );
        ADDWORD    ( pak, clientid );
        ADDDWORD   ( pak, GServer->BuildBuffs( this ) );
        GServer->SendToVisible( &pak, this );
    }
}

// VIRTUAL [return party pointer]
CParty* CCharacter::GetParty( )
{
    return NULL;
}

// VIRTUAL [return clan pointer]
CLAN* CCharacter::GetClan( )
{
    return NULL;
}

bool CCharacter::IsAttacking( )
{
    if(IsOnBattle( ))
    {
        switch(Battle->atktype)
        {
            case SKILL_BUFF:
            case BUFF_SELF:
            case BUFF_AOE:
                return false;
        }
    }
    else return false;
    return true;
}

