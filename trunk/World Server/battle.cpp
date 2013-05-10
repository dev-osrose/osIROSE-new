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

void CCharacter::DoAttack( )
{
    CMap* map = GServer->MapList.Index[Position->Map];
    switch(Battle->atktype)
    {
        case NORMAL_ATTACK://normal attack
        {
            CCharacter* Enemy = GetCharTarget( );
            if(Enemy==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            if(IsTargetReached( Enemy ) && CanAttack( ))
            {
                NormalAttack( Enemy );
                if (Enemy->IsMonster())
                {
                    CMonster* monster=GServer->GetMonsterByID(Enemy->clientid, Enemy->Position->Map);
                    monster->DoAi(monster->thisnpc->AI, 3);
                }
            }
        }
        break;
        case SKILL_ATTACK://skill attack
        {
            CCharacter* Enemy = GetCharTarget( );
            if(Enemy==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            CSkills* skill = GServer->GetSkillByID( Battle->skillid );
            if(skill==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            if(IsTargetReached( Enemy, skill ) && CanAttack( ))
            {
                SkillAttack( Enemy, skill );
                if (Enemy->IsMonster())
                {
                    CMonster* monster=GServer->GetMonsterByID(Enemy->clientid, Enemy->Position->Map);
                    monster->DoAi(monster->thisnpc->AI, 3);
                }
            }
        }
        break;
        case SKILL_BUFF://buffs
        {
            CCharacter* Enemy = GetCharTarget( );
            if(Enemy==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            CSkills* skill = GServer->GetSkillByID( Battle->skillid );
            if(skill==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            if(IsTargetReached( Enemy, skill ) && CanAttack( ))
                BuffSkill( Enemy, skill );
        }
        break;
        case SKILL_AOE:
        case SKILL_SELF:
        case AOE_TARGET:
        {
            CCharacter* Enemy = NULL;
            CSkills* skill = GServer->GetSkillByID( Battle->skillid );
            if(skill==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            if(Battle->atktype==AOE_TARGET)
            {
                float distance = GServer->distance( Position->current, Position->destiny );
                if(distance<=skill->range && CanAttack( )){
                    Position->destiny=Position->current;
                    AoeSkill( skill, NULL );
                    }
            }
            else
            {
                Position->skilltargetpos = Position->current;
                AoeSkill( skill, Enemy );
            }
        }
        break;
        case BUFF_SELF:
        {
            CSkills* skill = GServer->GetSkillByID( Battle->skillid );
            if(skill==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            BuffSkill( this, skill );
        }
        break;
        case BUFF_AOE:
        {
            CSkills* skill = GServer->GetSkillByID( Battle->skillid );
            if(skill==NULL)
            {
                ClearBattle( Battle );
                return;
            }
            AoeBuff( skill );
        }
        break;
    }
}

// do normal attack
void CCharacter::NormalAttack( CCharacter* Enemy )
{
    Enemy->OnBeAttacked( this );
    Position->destiny  = Position->current;
    long int hitpower = 0;
    bool critical = false;
    int hitvalue = GetHitRate(Enemy);

/*
    if (hitvalue < 20)
      if (int((float(Stats->Level - Enemy->Stats->Level) * 0.6f) + float((rand() % 100) + 1)) >= 94)
        hitvalue = 100;

    if (hitvalue >= 20) {
      // PY code
      float mod = 0;
      if(IsPlayer())
      {
          mod = 26 + (Stats->Level / 2);
      }
      else
      {
          mod = 26 + (Stats->Level / 3);
      }
      float attack = 0;
      if(Stats->magicattack == 1)
      {

          attack = (float) Stats->Attack_Power / Enemy->Stats->Magic_Defense;
          attack *= mod;
      }
      else
      {

          attack = (float) Stats->Attack_Power / Enemy->Stats->Defense;
          attack *= mod;
      }
      //PY end
      //original
      //float attack = (float)Stats->Attack_Power - ((Enemy->Stats->Magic_Defense+Enemy->Stats->Defense )/2);
      if(attack<5) attack = 5;
      //attack *= 0.65;
      float d_attack = attack / 100;
      if(attack<5) attack = 5;
      mod = GServer->RandNumber( 0, 10 ) * d_attack;
      attack += mod;

      hitpower = (long int)floor(attack + GServer->RandNumber(0, 10));
      if(IsPlayer( )) //temp fix to find balance btw monster and player
      {
          hitpower = (long int)floor(attack * (GServer->Config.PlayerDmg/100.00));
          hitpower+=((hitpower*(Stats->ExtraDamage))/100);
      }
      if(IsMonster( )) //temp fix to find balance btw monster and player
          hitpower = (long int)floor(attack * (GServer->Config.MonsterDmg/100.00));
      if(hitpower<=5)
      {
          hitpower = 5;
      }
      else
      {
          if(GServer->RandNumber(0,300)<Stats->Critical)
          {
              hitpower = (long int)floor(hitpower*1.5);
              critical = true;
          }
      }
      if (hitpower > 0x7ff)//2047 packet size limit.
      {
         hitpower=0x7ff;
      }
    }*/
      if (hitvalue < 20)
      if (int((float(Stats->Level - Enemy->Stats->Level) * 0.6f) + float((rand() % 100) + 1)) >= 94)
        hitvalue = 100;

    if (hitvalue >= 20) {
      bool magicattack = GetMagicAttack();
      if (magicattack)
        hitpower = GetMgcDmg(Enemy, 1, hitvalue);
      else
    	hitpower = GetAtkDmg(Enemy, 1, hitvalue);
      if (hitpower & CRITBIT) critical = true;
    }

    if(!Enemy->IsSummon( ) && Enemy->IsMonster( ))
    {
        Enemy->AddDamage( this, (hitpower & ~(CRITBIT | UNK1BIT)));
        Enemy->damagecounter+=hitpower;// is for AI
    }
    Enemy->Stats->HP -= (hitpower & ~(CRITBIT | UNK1BIT));
    BEGINPACKET( pak, 0x799 );
    ADDWORD    ( pak, clientid );
    ADDWORD    ( pak, Battle->atktarget );
#ifdef USE124
    ADDDWORD   ( pak, hitpower );
#endif

    if(Enemy->IsDead())
    {
        CDrop* thisdrop = NULL;
#ifndef USE124
        //ADDWORD ( pak, (hitpower |   (    critical?0xb000:0x8000   )    ));
        ADDWORD   ( pak, hitpower | KILLBIT );
#else
        ADDDWORD   ( pak, critical?28:16 );
#endif
        if(!Enemy->IsSummon( ) && !Enemy->IsPlayer( ))
        {
            thisdrop = Enemy->GetDrop( );
            if(thisdrop!=NULL)
            {
                ADDFLOAT   ( pak, thisdrop->pos.x*100 );
                ADDFLOAT   ( pak, thisdrop->pos.y*100 );
                if(thisdrop->type==1)
                {
                    ADDWORD( pak, 0xccdf );
                    ADDDWORD( pak, thisdrop->amount );
                }
                else
                {
                    ADDWORD   ( pak, GServer->BuildItemHead( thisdrop->item ) );
                    ADDDWORD   ( pak, GServer->BuildItemData( thisdrop->item ) );
                }
                ADDWORD    ( pak, thisdrop->clientid );
                ADDWORD    ( pak, thisdrop->owner );
                CMap* map = GServer->MapList.Index[thisdrop->posMap];
                map->AddDrop( thisdrop );
            }
        }
        GServer->SendToVisible( &pak, Enemy, thisdrop );
        OnEnemyDie( Enemy );
    }
    else
    {
#ifndef USE124
        //ADDWORD   ( pak, (hitpower|(hitpower>0?(critical?0x4000:0):0)));
        ADDWORD     ( pak, hitpower );
#else
        ADDDWORD   ( pak, (hitpower>0?(critical?12:0):0) );
#endif
        GServer->SendToVisible( &pak, Enemy );
    }
    ReduceABC( );
    Battle->lastAtkTime = clock( );
}

// do skill attack
bool CCharacter::SkillAttack( CCharacter* Enemy, CSkills* skill )
{
    Position->destiny = Position->current;
    if(Battle->castTime==0)
    {
        BEGINPACKET( pak, 0x7bb );
        ADDWORD    ( pak, clientid );
        GServer->SendToVisible( &pak, this );
        Battle->castTime = clock();
        return true;
    }
    else
    {
        clock_t etime = clock() - Battle->castTime;
        if(etime<SKILL_DELAY)
            return true;
    }
    Battle->castTime = 0;
    UseAtkSkill( Enemy, skill );
    Stats->MP -= (skill->mp - (skill->mp * Stats->MPReduction / 100));
    if(Stats->MP<0) Stats->MP=0;
    if(Battle->contatk)
    {
        Battle->atktype = NORMAL_ATTACK;
        Battle->skilltarget = 0;
        Battle->atktarget = Battle->target;
        Battle->skillid = 0;
    }
    else
    {
        ClearBattle( Battle );
    }
    GServer->DoSkillScript( this, skill );
    Battle->lastAtkTime = clock( );
    return true;
}

// do buff skill
bool CCharacter::BuffSkill( CCharacter* Target, CSkills* skill )
{
    Position->destiny = Position->current;
    if(Battle->castTime==0)
    {
        BEGINPACKET( pak, 0x7bb );
        ADDWORD    ( pak, clientid );
        GServer->SendToVisible( &pak, (CCharacter*)this );
        Battle->castTime = clock();
        return true;
    }
    else
    {
        clock_t etime = clock() - Battle->castTime;
        if(etime<SKILL_DELAY)
            return true;
    }
    Battle->castTime = 0;
    UseBuffSkill( Target, skill );
    Stats->MP -= (skill->mp - (skill->mp * Stats->MPReduction / 100));
    if(Stats->MP<0) Stats->MP=0;
    ClearBattle( Battle );
    GServer->DoSkillScript( this, skill );
    Battle->lastAtkTime = clock( );
    return true;
}
//arnold
// do AoE skill
bool CCharacter::AoeSkill( CSkills* skill, CCharacter* Enemy )
{
    Position->destiny = Position->current;
    if(Battle->castTime==0)
    {
        BEGINPACKET( pak, 0x7bb );
        ADDWORD    ( pak, clientid );
        GServer->SendToVisible( &pak, (CCharacter*)this );
        Battle->castTime = clock();
        return true;
    }
    else
    {
        clock_t etime = clock() - Battle->castTime;
        if(etime<SKILL_DELAY)
            return true;
    }
    Battle->castTime = 0;
    CMap* map = GServer->MapList.Index[Position->Map];
    for(UINT i=0;i<map->MonsterList.size();i++)
    {
        CMonster* monster = map->MonsterList.at(i);
        if(monster->clientid==clientid) continue;
        if(IsSummon( ) || IsPlayer( ))
        {
            if(monster->IsSummon( ) && (map->allowpvp==0 || monster->owner==clientid)) continue;
        }
        else
        {
            if(!monster->IsSummon( )) continue;
        }
        if(GServer->IsMonInCircle( Position->skilltargetpos,monster->Position->current,(float)skill->aoeradius+1))
            UseAtkSkill( (CCharacter*) monster, skill );
    }
    if(map->allowpvp!=0 || (IsMonster( ) && !IsSummon( )))
    {
        for(UINT i=0;i<map->PlayerList.size();i++)
        {
            CPlayer* player = map->PlayerList.at(i);
            if(player->clientid==clientid) continue;
            if(GServer->IsMonInCircle( Position->skilltargetpos,player->Position->current,(float)skill->aoeradius+1))
                UseAtkSkill( (CCharacter*) player, skill );
        }
    }
    if(Enemy!=NULL)
    {
        if(!Enemy->IsDead( ))
        {
            Battle->atktarget = Battle->target;
            Battle->atktype = NORMAL_ATTACK;
            Battle->skilltarget = 0;
            Battle->skillid = 0;
        }
        else ClearBattle( Battle );
    }
    else ClearBattle( Battle );
    Stats->MP -= (skill->mp - (skill->mp * Stats->MPReduction / 100));
    if(Stats->MP<0) Stats->MP=0;
    Battle->lastAtkTime = clock( );
    return true;
}

bool CCharacter::AoeBuff( CSkills* skill )
{
    Position->destiny = Position->current;
    if(Battle->castTime==0)
    {
        BEGINPACKET( pak, 0x7bb );
        ADDWORD    ( pak, clientid );
        GServer->SendToVisible( &pak, (CCharacter*)this );
        Battle->castTime = clock();
        return true;
    }
    else
    {
        clock_t etime = clock() - Battle->castTime;
        if(etime<SKILL_DELAY)
            return true;
    }
    Battle->castTime = 0;
    CMap* map = GServer->MapList.Index[Position->Map];
    if( (skill->target == tPartyMember && GetParty() == NULL) ||
        (skill->target == tClanMember && GetClan() == NULL) )
    {
        UseBuffSkill( this, skill );
	    Stats->MP -= (skill->mp - (skill->mp * Stats->MPReduction / 100));
	    if(Stats->MP<0) Stats->MP=0;
        ClearBattle( Battle );
        Battle->lastAtkTime = clock( );
        return true;
    }
    if(CharType==1)
    {
        for(UINT i=0;i<map->PlayerList.size();i++)
        {
            CPlayer* player = map->PlayerList.at(i);
            switch(skill->target)
            {
                case tPartyMember: // party

                    if(player->Party->party==GetParty( ))
                    {
                        if(GServer->IsMonInCircle( Position->current,player->Position->current,(float)skill->aoeradius+1))
                            UseBuffSkill( (CCharacter*)player, skill );
                    }
                break;
                case tClanMember:
                  if (player->Clan == GetClan( ))
                {
                    if(GServer->IsMonInCircle( Position->current, player->Position->current, (float)skill->aoeradius + 1 ) )
                        UseBuffSkill( (CCharacter*)player, skill );
                  }
                break;
                case tAlly:
                case tAllCharacters:
                case tAllMembers: // all characters
                {
                     if(GServer->IsMonInCircle( Position->current,player->Position->current,(float)skill->aoeradius+1))
                            UseBuffSkill( (CCharacter*)player, skill );
                }
                break;
                case tHostileCharacter:
                {
                     if (CharType==2)
                     {
                        if(GServer->IsMonInCircle( Position->current,player->Position->current,(float)skill->aoeradius+1))
                        UseBuffSkill( (CCharacter*)player, skill );
                     }
                }
                break;

            }
        }
    }
    else if (CharType==2)
    {
        for(UINT i=0;i<map->MonsterList.size();i++)
        {
            CMonster* monster = map->MonsterList.at(i);
            switch(skill->target)
            {
                case tPartyMember: // party
                break;
                case tClanMember:
                break;
                case tAlly:
                case tAllCharacters:
                case tAllMembers: // all characters
                {
                     if(GServer->IsMonInCircle( Position->current,monster->Position->current,(float)skill->aoeradius+1))
                            UseBuffSkill( (CCharacter*)monster, skill );
                }
                break;
                case tHostileCharacter:
                break;

            }
        }
        Battle->lastAtkTime = clock( );
        return true;
    }
    Stats->MP -= (skill->mp - (skill->mp * Stats->MPReduction / 100));
    if(Stats->MP<0) Stats->MP=0;
    ClearBattle( Battle );
    Battle->lastAtkTime = clock( );
    return true;
}



// use skill attack
void CCharacter::UseAtkSkill( CCharacter* Enemy, CSkills* skill, bool deBuff )
{
    /*
    float attack2 = (float)Stats->Attack_Power - (Enemy->Stats->Defense );
    //attack2 -= (float)Stats->Attack_Distance*10 ;
    if(attack2<5) attack2 = 5;
    long int hitpower2 = (long int)floor(attack2);
    if(IsPlayer( )) //temp fix to find balance btw skillattack and normal attack - littlelilly
    {    hitpower2 = (long int)floor(attack2 * (100/100.00));
         hitpower2 += GServer->RandNumber(0,20);
    */
    //Skill power calculations - Pepu
    long int skillpower = skill->atkpower + (long int)floor(GetInt( )/2);
    long int level_diff = Stats->Level - Enemy->Stats->Level;



    long int apchar = Stats->Attack_Power; //apchar neccesary, because if i use stats->Attack_Power will change normal attack code
    if (level_diff >= 10) apchar = (long int)(apchar*1.2);
    if (level_diff >= 20) apchar = (long int)(apchar*1.4);
    if (level_diff >= 30) apchar = (long int)(apchar*1.7);
    if (level_diff >= 40) apchar = (long int)(apchar*2);
    if (level_diff <= 10) apchar = (long int)(apchar*0.9);
    if (level_diff <= 20) apchar = (long int)(apchar*0.8);


    if(skill->skilltype == 6 || skill->skilltype == 7)
    {
    skillpower = (long int)((skillpower + apchar*2 + ( Stats->Attack_Power - Enemy->Stats->Magic_Defense ) - Enemy->Stats->Magic_Defense*2 + level_diff) / 1.8);
    skillpower += GServer->RandNumber(0,20);
    }
    else{
    skillpower = (long int)((skillpower + apchar*2 + ( Stats->Attack_Power - Enemy->Stats->Defense ) - Enemy->Stats->Magic_Defense*2 + level_diff) / 2.4);
    skillpower += GServer->RandNumber(0,20);
    }

    if (skillpower >=2047) skillpower= 2047;
    if(this->CharType==2)skillpower=30;//aquas kill you in one hit with above calculations
    //Tell enemy he's attacked & add damage & send the dmg packet
    bool bflag = false;
    Enemy->OnBeAttacked( this );
    if(skillpower >= 5000000) skillpower = 5;  //if ever computations got wild- this will avoid having crazy damage -littlelilly
    if(skill->atkpower <= 5) skillpower = 0;   //so that skills having 0 attack have no damage at all - littlelilly
    if(!Enemy->IsSummon( ) && Enemy->IsMonster( ))
    {
        Enemy->AddDamage( this, skillpower );
        Enemy->damagecounter+=skillpower;
    }

    Enemy->Stats->HP -= skillpower;

    BEGINPACKET( pak, 0x7b6 );
    ADDWORD    ( pak, Enemy->clientid );
    ADDWORD    ( pak, clientid );
    if (Enemy->IsMonster()){
         ADDWORD( pak, Battle->skillid);
         ADDBYTE( pak, 0x00);
     }
    else {
         ADDWORD( pak, (Battle->skillid+0x1000)+((GetInt()%4)*0x4000) );
         ADDBYTE    ( pak, GetInt( )/4 );
    }
    if (Enemy->Stats->HP<1)
    {
        ADDWORD   ( pak, skillpower+0x8000 );
    }
    else
    {
        if (Enemy->IsMonster()){ADDWORD   ( pak, skillpower );}
        else {ADDWORD( pak, skillpower+0x2000 );}
    }
    if(Enemy->IsDead())
    {
        CDrop* thisdrop = NULL;
        //??????ADDDWORD   ( pak, 16 );
        if(!Enemy->IsSummon( ) && !Enemy->IsPlayer( ))
        {
            thisdrop = Enemy->GetDrop( );
            if(thisdrop!=NULL)
            {
                ADDFLOAT   ( pak, thisdrop->pos.x*100 );
                ADDFLOAT   ( pak, thisdrop->pos.y*100 );
                if(thisdrop->type==1)
                {
                    ADDWORD( pak, 0xccdf );
                    ADDDWORD( pak, thisdrop->amount );
                }
                else
                {
                    ADDWORD   ( pak, GServer->BuildItemHead( thisdrop->item ) );
                    ADDDWORD   ( pak, GServer->BuildItemData( thisdrop->item ) );
                }
                ADDWORD    ( pak, thisdrop->clientid );
                ADDWORD    ( pak, thisdrop->owner );
                CMap* map = GServer->MapList.Index[thisdrop->posMap];
                map->AddDrop( thisdrop );
            }
        }
        GServer->SendToVisible( &pak, Enemy, thisdrop );
        OnEnemyDie( Enemy );
    }
    else
    {
        ADDDWORD   ( pak, 4 );
        GServer->SendToVisible( &pak, Enemy );

        if (deBuff) return;

        //GOTO debuffing section
        //bflag = GServer->AddDeBuffs( skill, Enemy, GetInt( ) ); // should never need this now that the buffs have been reworked.
        bflag = GServer->AddBuffs( skill, Enemy, GetInt( ) ); // send to AddBuffs instead.

        //Send (de)buff information to the whole world
        if(skill->nbuffs>0 && bflag)
        {
            BEGINPACKET( pak, 0x7b5 );
            ADDWORD    ( pak, Battle->skilltarget );
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, (Battle->skillid+0x1000)+((GetInt()%4)*0x4000) );
            ADDBYTE    ( pak, GetInt( )/4 );
            //ADDBYTE    ( pak, skill->nbuffs );
            GServer->SendToVisible( &pak, Enemy );
        }
    }
    if (deBuff) return;
    //Send skill animation to the world
    RESETPACKET( pak, 0x7b9);
    ADDWORD    ( pak, clientid);
    ADDWORD    ( pak, Battle->skillid);
    //ADDWORD    ( pak, 1);
    GServer->SendToVisible( &pak, this );
}

// use buff skill
void CCharacter::UseBuffSkill( CCharacter* Target, CSkills* skill )
{
    bool bflag = false;
    bflag = GServer->AddBuffs( skill, Target, GetInt( ) );
    if(skill->nbuffs>0 && bflag == true)
    {
        BEGINPACKET( pak, 0x7b5 );
        ADDWORD    ( pak, Target->clientid );
        ADDWORD    ( pak, clientid );
        if (skill->nbuffs>1)//???????????
        {
            ADDWORD    ( pak, Battle->skillid+0x3000+((GetInt()%4)*0x4000) );//maybe number of buffs?
        }
        else{
        ADDWORD    ( pak, (Battle->skillid+0x1000)+((GetInt()%4)*0x4000) );}
        ADDBYTE    ( pak, GetInt( )/4 );
        GServer->SendToVisible( &pak, Target );
    }
    BEGINPACKET( pak, 0x7b9);
    ADDWORD    ( pak, clientid);
    ADDWORD    ( pak, Battle->skillid);
    //ADDWORD    ( pak, 1);
	GServer->SendToVisible( &pak, this );
}

// Use a skill
bool CCharacter::UseSkill( CSkills* skill, CCharacter *Target )
{
  if (skill->atkpower > 0)
  {
    Log(MSG_INFO, "Need to do %i%s %s in range %i to target %i",
        ( skill->atkpower ),
        ( (skill->range > 0) ? " AOE" : "" ),
        ( (GServer->isSkillTargetFriendly(skill)) ? "healing" : "damage" ),
        ( skill->range ),
        ( skill->target ) );
  }
  for (int i = 0; i < 2; i++) {
    Log(MSG_INFO, "Status ID: %i", skill->status[i]);
    if (skill->status[i] == 0) continue;
    CStatus* status = GServer->GetStatusByID(skill->status[i]);
    if (status == NULL) continue;
    /************************
    We'll probably need to use status->decrease (Figure out whether buf is up/down
    status->repeat will tell us whether it's a one-time (Stat Boost [2]), repeat
    (Recovery, continueing damage [1]) or Special (Status Effect [3]).
    ************************/
    if (status->repeat == 1) // Continuous
    {
      Log(MSG_INFO, "Need to take stat %i and %s it by %i%s over %i seconds", skill->buff[i],
          ((status->decrease) ? "decrease" : "increase"),
          ((skill->value1[i] != 0) ? skill->value1[i] : skill->value2[i]),
          ((skill->value1[i] != 0) ? "" : "%"), skill->duration);

    } else if (status->repeat == 2) // Once (Stat Boost)
    {
      Log(MSG_INFO, "Need to take stat %i and %s it by %i%s for %i seconds", skill->buff[i],
          ((status->decrease) ? "decrease" : "increase"),
          ((skill->value1[i] != 0) ? skill->value1[i] : skill->value2[i]),
          ((skill->value1[i] != 0) ? "" : "%"), skill->duration);

    } else if (status->repeat == 3) // Status Effect (Poison,etc)
    {
      Log(MSG_INFO, "Need to give user status effect %i for %i seconds", skill->buff[i],
          ((skill->value1[i] != 0) ? skill->value1[i] : skill->value2[i]),
          ( skill->duration ) );

    }
  }
  return true;
}

int CCharacter::GetHitRate(CCharacter* pDEF) {
  int iRAND1, iRAND2, iSuccess;
  if ( IsPlayer() && pDEF->IsPlayer() ) {
    float tempFloat = rand() % 180;
    tempFloat += ((float(pDEF->GetDodge()) * 0.7f) - float(GetAccury())) + 1.0f;
    tempFloat += 10.0f;
    tempFloat *= 0.2f;
    return int(tempFloat);
  } else {
    iRAND1 = rand() % 50 + 1;
    iRAND2 = rand() % 60 + 1;

    float tempFloat = pDEF->Stats->Level;
    tempFloat *= 1.1f;
    tempFloat = (Stats->Level + 10) - tempFloat;
    tempFloat += iRAND1;

    iSuccess = int(tempFloat);
		if (iSuccess <= 0) return 0;
    tempFloat = pDEF->GetDodge();
    tempFloat *= 0.93f;
    tempFloat = (float(GetAccury()) * 1.1f) - tempFloat;
    tempFloat += iRAND2;
    tempFloat += 5.0f;
    tempFloat += float(Stats->Level) * 0.2f;
    tempFloat *= float(iSuccess);
    tempFloat /= 80.0f;
    return int(tempFloat);
  }
}

int CCharacter::GetCritRate(){
	int critRate = 3 * (rand() % 100 + 1);
	critRate = 16 * (critRate + Stats->Level + 30);
	critRate = critRate / (GetCritical() + 70);
	return critRate;
}

int CCharacter::GetAtkDmg(CCharacter* pDEF, word wHitCNT, int iSuc){
  int curDamage = 0;
  int critSuccessRate = GetCritRate();
  int unk1 = (((int)GetAttackPower() + 20) * (28 - critSuccessRate)) / ((int)pDEF->GetDefense() + 5);

  if( critSuccessRate >= 20 ) {
		if( IsPlayer() && pDEF->IsPlayer() ) {
			float tempFloat = ((iSuc * 0.05f) + 25) * (int)GetAttackPower();
      tempFloat *= (int)GetAttackPower() - (int)pDEF->GetDefense() + 400;

			curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.4f) + (int)pDEF->GetDefense() + 5.0f) * 420.0f) + 20);
    } else {
			float tempFloat = ((iSuc * 0.03f) + 26) * (int)GetAttackPower();
      tempFloat *= (int)GetAttackPower() - (int)pDEF->GetDefense() + 250;

			curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.4f) + (int)pDEF->GetDefense() + 5.0f) * 145.0f));
    }
/*// Buff up stuff, Add - Drakia
    if( pATK->GetIngDurationStateFLAG() & 0x10000000 ){//Has DAMAGE_UP buff
			//LIST_SKILL STB! (row from func, column 9 (36 / 4 = 9)?)
			//Get Skill ID of DAMAGE_UP BUFF
			//Get Skill Power of it!
      int unk2 = *(_DWORD *)(*(_DWORD *)(dword_100CC038 + 4 * pATK->GetIngDurationStateSKILL(28)) + 36) * curDamage;
      curDamage += int(float(unk2) / 100.0f);
    }
*/
    if (IsPlayer())
      curDamage *= (int)floor(GServer->Config.PlayerDmg / 100.00);
    else
      curDamage *= (int)floor(GServer->Config.MonsterDmg / 100.00);

    curDamage *= wHitCNT;
    if(curDamage >= 5) {
			if( IsPlayer() && pDEF->IsPlayer() ) {
        int maxDamage = (int)(pDEF->GetMaxHP() * 0.25f);
        if( curDamage > maxDamage )
          curDamage = maxDamage;
      } else {
        if( curDamage > 2047 )
          curDamage = 2047;
      }
    } else {
      curDamage = 5;
    }
	} else {
    if( IsPlayer() && pDEF->IsPlayer() ) {
      float tempFloat = ((iSuc * 0.05f) + 35) * (int)GetAttackPower();
      tempFloat *= (int)GetAttackPower() - (int)pDEF->GetDefense() + 430;

      curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.4f) + (int)pDEF->GetDefense() + 10) * 300.0f) + 25);
    } else {
      float tempFloat = ((iSuc * 0.05f) + 29.0f) * (int)GetAttackPower();
      tempFloat *= (((int)GetAttackPower() - (int)pDEF->GetDefense()) + 230);

      curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.3f) + (int)pDEF->GetDefense() + 5) * 100.0f));
    }
/*// Buff up stuff - Drakia
    if( pATK->GetIngDurationStateFLAG() & 0x10000000 ){//Has DAMAGE_UP buff
			//LIST_SKILL STB! (row from func, column 9 (36 / 4 = 9)?)
			//Get Skill ID of DAMAGE_UP BUFF
			//Get Skill Power of it!
      int unk2 = *(_DWORD *)(*(_DWORD *)(dword_100CC038 + 4 * (signed __int16)pATK->GetIngDurationStateSKILL(28)) + 36) * curDamage;
      curDamage += int(float(unk2) / 100.0f);
    }
*/
    if (IsPlayer())
      curDamage *= (int)floor(GServer->Config.PlayerDmg / 100.00);
    else
      curDamage *= (int)floor(GServer->Config.MonsterDmg / 100.00);

    curDamage *= wHitCNT;

    if( curDamage >= 10 ) {
      if( IsPlayer() && pDEF->IsPlayer() ) {
        int maxDamage = (int)(pDEF->GetMaxHP() * 0.35f);
        if( curDamage > maxDamage )
          curDamage = maxDamage;
      } else {
        if( curDamage > 2047 )
          curDamage = 2047;
      }
    } else {
      curDamage = 10;
    }
    curDamage |= 0x4000;
  }
  // Wow, I have no fucking clue what this does - Drakia
  if( unk1 < 10 )
    return curDamage;
  else
    return curDamage | 0x2000;
}

int CCharacter::GetMgcDmg(CCharacter* pDEF, word wHitCNT, int iSuc){
  	int curDamage = 0;
	int critSuccessRate = GetCritRate();
	int unk1 = ((GetAttackPower() + 20) * (28 - critSuccessRate)) / (pDEF->GetMagicDefense() + 5);

	if(critSuccessRate >= 20) {
		if(IsPlayer() && pDEF->IsPlayer()) {
			float tempFloat = ((iSuc * 0.06f) + 29) * (int)GetAttackPower();
			tempFloat *= (int)GetAttackPower() - ((int)pDEF->GetDefense() * 0.8f) + 350;

			curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.3f) + (int)pDEF->GetMagicDefense() + 5) * 640.0f) + 20);
		} else {
			float tempFloat = ((iSuc * 0.03f) + 30) * (int)GetAttackPower();
			tempFloat *= (int)GetAttackPower() - ((int)pDEF->GetDefense() * 0.8f) + 280;

			curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.3f) + (int)pDEF->GetMagicDefense() + 5) * 280.0f));
		}
/* // Buff, add later - Drakia
		if( pATK->GetIngDurationStateFLAG() & 0x10000000 ){//Has DAMAGE_UP buff
			//LIST_SKILL STB! (row from func, column 9 (36 / 4 = 9)?)
			//Get Skill ID of DAMAGE_UP BUFF
			//Get Skill Power of it!
			int unk2 = *(_DWORD *)(*(_DWORD *)(dword_100CC038 + 4 * pATK->GetIngDurationStateSKILL(28)) + 36) * curDamage;
			curDamage += int(float(unk2) / 100.0f);
		}
*/

    if (IsPlayer())
      curDamage *= (int)floor(GServer->Config.PlayerDmg / 100.00);
    else
      curDamage *= (int)floor(GServer->Config.MonsterDmg / 100.00);

		curDamage *= wHitCNT;
		if(curDamage >= 5) {
			if( IsPlayer() && pDEF->IsPlayer() ) {
				int maxDamage = (int)(pDEF->GetMaxHP() * 0.25f);
				if( curDamage > maxDamage )
					curDamage = maxDamage;
			} else {
				if( curDamage > 2047 )
					curDamage = 2047;
			}
		} else {
			curDamage = 5;
		}
	} else {
		if ( IsPlayer() && pDEF->IsPlayer() ) {
			float tempFloat = ((iSuc * 0.8f) + 33) * (int)GetAttackPower();
			tempFloat *= (int)GetAttackPower() - (int)pDEF->GetDefense() + 340;

			curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.3f) + (int)pDEF->GetMagicDefense() + 20) * 360.0f)) + 25;
		} else {
			float tempFloat = ((iSuc * 0.05f) + 33) * (int)GetAttackPower();
			tempFloat *= (int)GetAttackPower() - (int)pDEF->GetDefense() + 310;

			curDamage = (int)(tempFloat / ((((int)pDEF->GetDodge() * 0.3f) + (int)pDEF->GetMagicDefense() + 5) * 200.0f));
		}
/* // Buff - Add Later - Drakia
		if( pATK->GetIngDurationStateFLAG() & 0x10000000 ){//Has DAMAGE_UP buff
			//LIST_SKILL STB! (row from func, column 9 (36 / 4 = 9)?)
			//Get Skill ID of DAMAGE_UP BUFF
			//Get Skill Power of it!
			int unk2 = *(_DWORD *)(*(_DWORD *)(dword_100CC038 + 4 * pATK->GetIngDurationStateSKILL(28)) + 36) * curDamage;
			curDamage += int(float(unk2) / 100.0f);
		}
*/

    if (IsPlayer())
      curDamage *= (int)floor(GServer->Config.PlayerDmg / 100.00);
    else
      curDamage *= (int)floor(GServer->Config.MonsterDmg / 100.00);

		curDamage *= wHitCNT;
		if( curDamage >= 10 ){
			if( IsPlayer() && pDEF->IsPlayer() ){
				int maxDamage = (int)(pDEF->GetMaxHP() * 0.35f);
				if( curDamage > maxDamage )
					curDamage = maxDamage;
			}else{
				if( curDamage > 2047 )
					curDamage = 2047;
			}
		}else{
			curDamage = 10;
		}
		curDamage |= 0x4000;
	}
	if( unk1 < 10 )
		return curDamage;
	else
		return curDamage | 0x2000;
}
