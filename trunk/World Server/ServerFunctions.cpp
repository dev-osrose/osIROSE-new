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
#include "worldserver.h"

// from Paul_T
bool CWorldServer::SendPM( CPlayer* thisclient, char* Format, ... )
{
  char buf[512];
  va_list ap;
  va_start( ap, Format );
  vsprintf( buf, Format, ap );
           BEGINPACKET( pak, 0x0784 );
           ADDSTRING( pak, "Server" );
           ADDBYTE( pak, 0 );
     ADDSTRING( pak, buf );
           ADDBYTE( pak, 0 );
           thisclient->client->SendPacket(&pak);
  va_end( ap );
           return true;
}

// from Paul_T
bool CWorldServer::SendGlobalMSG( CPlayer* thisclient, char* Format, ... )
{
  char buf[512];
  va_list ap;
  va_start( ap, Format );
  vsprintf( buf, Format, ap );
           BEGINPACKET( pak, 0x0784 );
           ADDSTRING( pak, thisclient->CharInfo->charname );
           ADDBYTE( pak, 0 );
     ADDSTRING( pak, buf );
           ADDBYTE( pak, 0 );
           SendToAll(&pak);
  va_end( ap );
           return true;
}

bool CWorldServer::DoSkillScript( CCharacter* character, CSkills* thisskill )
{
     try
     {
        if(thisskill==0)
            return false;
        if(thisskill->script==0)
            return false;
        if(thisskill->svalue1==0)
            return false;
        fPoint position = RandInCircle( character->Position->current, 5 );
        CMap* map = MapList.Index[character->Position->Map];
        map->AddMonster( thisskill->svalue1, position, character->clientid );
        return true;
    }
    catch (...)
    {
         Log(MSG_WARNING, "Error in CWorldServer::DoSkillScript( CCharacter* character, CSkills* thisskill )(serverFunctions.cpp)" );
         return false;
    }
}

// Build Drop
CDrop* CWorldServer::GetDrop( CMonster* thismon )
{
    try{
    CDrop* newdrop = new (nothrow) CDrop;
    if(newdrop==NULL)
    {
        Log(MSG_WARNING, "Error allocing memory [getdrop]" );
        return NULL;
    }
    newdrop->clientid = GetNewClientID( );
    newdrop->posMap = thismon->Position->Map;
    newdrop->pos = RandInCircle( thismon->Position->current, 3 );
    newdrop->droptime = time(NULL);
    newdrop->owner = thismon->MonsterDrop->firsthit;
    newdrop->thisparty = thismon->thisparty;
    ClearItem(newdrop->item);
    int randv = RandNumber( 1, 100);
    if(thismon->MonsterDrop->mapdrop->level_max<thismon->MonsterDrop->firstlevel) randv = 100;
    if(randv<=30)//30% zuly [zulies will count as mapdrop]
    {
        if(thismon->MonsterDrop->mapdrop->level_max>=thismon->MonsterDrop->firstlevel)
        {
            newdrop->type = 1; //Drop Zuly
            newdrop->amount = thismon->thisnpc->level*5*Config.ZULY_RATE + RandNumber( 1, 10 );
            return  newdrop;
        }
        delete newdrop;
        return NULL;
    }
    CMDrops* thisdrops;
    newdrop->type = 2; //drop item drop
    switch( Config.DROP_TYPE )
    {
        case 0://only map
            thisdrops = thismon->MonsterDrop->mobdrop;
            if(thisdrops->level_max<thismon->MonsterDrop->firstlevel)
            {
                delete newdrop;
                return NULL;
            }
        break;
        case 1://mob only
            thisdrops = thismon->MonsterDrop->mapdrop;
            if(thismon->thisnpc->level-thismon->MonsterDrop->firstlevel<-14)
            {
                delete newdrop;
                return NULL;
            }
        break;
        default://both
            randv = RandNumber(1,100);
            if(thismon->MonsterDrop->mapdrop!=NULL)
                if(thismon->MonsterDrop->mapdrop->level_max<thismon->MonsterDrop->firstlevel)
                    randv = 100;
            if(randv>60)//select wich drop will use (map or mob) //40 - 60%
            {
                thisdrops = thismon->MonsterDrop->mobdrop;
                if((int)(thismon->thisnpc->level-thismon->MonsterDrop->firstlevel) < -14)
                {
                    delete newdrop;
                    return NULL;
                }
            }
            else
            {
                thisdrops = thismon->MonsterDrop->mapdrop;
                if(thisdrops->level_max<thismon->MonsterDrop->firstlevel)
                {
                    delete newdrop;
                    return NULL;
                }
            }
        break;
    }
    if(thisdrops==NULL)
    {
        thisdrops = thismon->MonsterDrop->mobdrop;
        if(thisdrops==NULL)
        {
            thisdrops = thismon->MonsterDrop->mapdrop;
            if(thisdrops==NULL)
            {
                newdrop->type = 1; //Drop Zuly
                newdrop->amount = thismon->thisnpc->level*5*Config.ZULY_RATE - RandNumber( 1, 20 );
                return  newdrop;
            }
        }
    }
    randv = 0;
    randv = RandNumber( 1, thisdrops->probmax );
    DWORD prob = 1;
    for(UINT i=0;i<thisdrops->Drops.size();i++)
    {
        CDropInfo* dropinfo = thisdrops->Drops.at( i );
        prob += dropinfo->prob;
        if(randv<=prob)
        {
            newdrop->item.itemtype = dropinfo->type;
            newdrop->item.itemnum = dropinfo->item;
            break;
        }
    }
    if(newdrop->item.itemtype==0)
    {
        Log(MSG_WARNING, "Drop Probability Highter: %i", randv );
        delete newdrop;
        return NULL;
    }
    if(newdrop->item.itemtype>1 && newdrop->item.itemtype<10 && newdrop->item.itemtype!=JEWEL)
    {
        // Items drop with 0 refine
        newdrop->item.refine = 0;
        newdrop->item.lifespan = RandNumber( 30, 100 );
        newdrop->item.durability = RandNumber( 35, 70 );
        newdrop->item.socketed = false;
        randv = RandNumber( 1, 100 );
        if( randv < 30 )        // 30%
            newdrop->item.stats = rand()%300;
        newdrop->item.appraised = newdrop->item.stats==0?true:false;
    }
    else
    {
         newdrop->item.lifespan = 100;
         newdrop->item.durability = 40;
         newdrop->item.socketed = false;
         newdrop->item.stats = 0;
    }
    newdrop->item.count = 1;
    if( newdrop->item.itemtype == 10 || newdrop->item.itemtype == 12 )
    {
        newdrop->item.count = RandNumber( 1, 3 );
    }
    if(newdrop->item.durability>100)newdrop->item.durability=100;
    if(newdrop->item.lifespan>100)newdrop->item.lifespan=100;
    newdrop->item.gem = 0;
    return newdrop;
    }
    catch(...)
    {
       Log(MSG_WARNING, "Error in CWorldServer::GetDrop( CMonster* thismon )(serverFunctions.cpp)" );
       return NULL;
    }
}


// Build Drop the PY way
CDrop* CWorldServer::GetPYDrop( CMonster* thismon, UINT droptype )
{   //if droptype = 1 then it is a normal drop. if it is 2 then it is a potential side drop.
    //Log( MSG_INFO, "PYDrops function selected" );
    //Log( MSG_INFO, "monster is %i", thismon->montype );
    if(droptype == 2) // monster is still alive
    {
        // this part of the function reserved for the later addition of side drops
        //return NULL;  //temporary bypass for the side drop function
        // kicks it straight back if the monster is not dead
        if(thismon->thisnpc->side != 0) //perhaps we get a side drop??
        {
            if(GServer->RandNumber(0,100) < thismon->thisnpc->sidechance)
            {
                droptype = thismon->thisnpc->side;
            }
            else
            {
                return NULL;  //No drop this time
            }
        }
        else
        {
            return NULL;  //No drop this time
        }
    }
    CDrop* newdrop = new (nothrow) CDrop;
    if(newdrop==NULL)
    {
        Log(MSG_WARNING, "Error allocing memory [getdrop]" );
        return NULL;
    }
    newdrop->clientid = GetNewClientID( );
    newdrop->posMap = thismon->Position->Map;
    newdrop->pos = RandInCircle( thismon->Position->current, 3 );
    newdrop->droptime = time(NULL);
    newdrop->owner = thismon->MonsterDrop->firsthit;
    newdrop->thisparty = thismon->thisparty;
    ClearItem(newdrop->item);

    CPlayer* thisclient = GServer->GetClientByCID(thismon->MonsterDrop->firsthit);

    // code to modify drop chance for different levels
    //float charm = 0;
    //float droprate = thismon->thisnpc->dropchance;
    float droprate = Config.DROP_RATE;
    float leveldif = (float)thismon->thisnpc->level - (float)thisclient->Stats->Level;
    float dropchance = (droprate + (droprate * 0.01 * leveldif));
    if(dropchance < 10) dropchance = 10; //always a small chance of a drop even when the mob is more than 20 levels beneath your own
    if(thismon->thisnpc->level == 1)
        dropchance = 80;
    if (GServer->RandNumber(0, 100)> dropchance)
        return NULL; // no drop here. not this time anyway.

    CItemType prob[MDropList.size()];
    bool isdrop = false;
    int n = 0;
    int test = 0;
    long int probmax = 0;
    int itemnumber[MDropList.size()];
    int itemtype[MDropList.size()];
    int probability[MDropList.size()];
    int alternate[MDropList.size()][8];

    if( thismon->IsGhost())
    {
        // Stuff to do if the mob is a ghost of any type
        int selection = 1 + rand()%10;
        if( selection <= 5 )
        {
            newdrop->type = 1; //Drop Zuly.
            newdrop->amount = thismon->thisnpc->level * 10 * Config.ZULY_RATE + RandNumber( 1, 10 );
            return  newdrop;
        }
        else
        {
            for(int i=0; i<SkillbookList.size( ); i++)
            {
                newdrop->type = 2;
                CMDrops* thisdrop = GServer->SkillbookList.at(i);
                if(thisdrop->level_min <= thismon->thisnpc->level &&  thisdrop->level_max >= thismon->thisnpc->level)
                {
                    itemnumber[n] = thisdrop->itemnum;
                    itemtype[n] = thisdrop->itemtype;
                    probability[n] = thisdrop->prob;
                    probmax += thisdrop->prob;
                    n++;
                }
            }
        }
    }
    else // Stuff to do if the mob isn't a ghost
    {
        int dropmode = 0;
        int randv = RandNumber( 1, 100);
        // Each monster has its own rates for zuly and item drops defined in the database
        if(randv > thismon->thisnpc->item + thismon->thisnpc->money) return NULL; // did not qualify to drop anything this time
        if(randv <= thismon->thisnpc->money) // zuly drop instead of item drop
        {
            newdrop->type = 1;
            newdrop->amount = thismon->thisnpc->level * 5 * Config.ZULY_RATE + RandNumber( 1, 10 );
            return  newdrop;
        }
        // this means it is an item drop
        randv = RandNumber( 1, 100);
        if(randv > 70) // 30% map drop
        {
            dropmode = 1; // map drop selected
        }
        else if(randv > 30) // 40% mob drop
        {
            dropmode = 2; //mob drop selected
        }
        else // 30% level drop
        {
            dropmode = 3; //leveldrop selected
        }

        int randomdrop = GServer->RandNumber(1, 100);
        for(int i=0; i<MDropList.size( ); i++)
        {
            isdrop=false;
            CMDrops* thisdrop = GServer->MDropList.at(i);
            if(thisdrop->mob == thismon->montype && dropmode == 2) // monster drop
            {
                test = GServer->RandNumber(1, 1000);
                if(test < thisdrop->prob)
                {
                    isdrop = true;
                }
            }
            if(thisdrop->map == thismon->Position->Map && dropmode == 1) // map drop
            {
                test = GServer->RandNumber(1, 1000);
                if(thismon->thisnpc->level == 1)
                   test = GServer->RandNumber(1, 10000); // make it less likely to get map drops from event mobs
                if(test < thisdrop->prob)
                {
                    isdrop = true;
                }
            }
            if(thismon->thisnpc->level >= thisdrop->level_min && thismon->thisnpc->level <= thisdrop->level_max && dropmode == 3)
            {
                //Log(MSG_INFO, "Level drop selected. type %i number %i", thisdrop->itemtype, thisdrop->itemnum );
                test = GServer->RandNumber(1, 1000);
                if(test < thisdrop->prob)
                {
                    isdrop = true;
                }
                //else
            }
            if(isdrop == true)
            {
                if(droptype != 1) //side drops only. Skip if the item is not a match for side type
                {
                    if(itemtype[n] != droptype)continue;
                }
                //droptype 1 is a regular drop
                itemnumber[n] = thisdrop->itemnum;
                itemtype[n] = thisdrop->itemtype;
                //probability[n] = thisdrop->prob;
                alternate[n][0] = 0;
                for(int i=1;i<8;i++)
                {
                    alternate[n][i] = thisdrop->alt[i];
                }
                n++;
            }
        }
    }
    int newn = n;
    if(n == 0)
        return NULL;
    int maxitems = n;
    // randomize the item from the list
    n = GServer->RandNumber(0, maxitems);
    newdrop->item.itemnum = itemnumber[n];
    newdrop->item.itemtype = itemtype[n];
    newdrop->type = 2;

    newdrop->item.lifespan = 10 + rand()%80;
    float dmod = 0; //random number from 0 to 100 made up of 4 sub numbers to keep
    //the average value near to 50
    for(int i=0; i<4; i++)
    {
        float r1 = rand()%20;
        dmod += r1;
    }
    newdrop->item.durability = 10 + (int)dmod;
    if( newdrop->item.itemtype == 8 || newdrop->item.itemtype == 9 )
    {
        //This probability is now configurable from WorldServer.conf
        int psocked = rand()%101; //Probability of finding a socketed item
        if( psocked < Config.SlotChance) //default should be around 5% needs to be rare
        {
            newdrop->item.socketed = true;
        }
        else
        {
             newdrop->item.socketed = false;
        }
    }
    else
    {
        newdrop->item.socketed = false;
    }
    //Log( MSG_INFO, "Socket are OK");
    newdrop->item.appraised = false;
    newdrop->item.stats = 0;
    newdrop->item.count = 1;

    //int chamod = 0;
    int chamod = (int)floor(thisclient->Attr->Cha / 30);
    if(chamod <0) chamod = 0;
    int basedrop = 6 + chamod; //Base number of items to be dropped. add CHA to increase this.
    if( newdrop->item.itemtype == 10 || newdrop->item.itemtype == 12 )
    {
        newdrop->item.count = RandNumber(0, basedrop);
        if(thismon->thisnpc->level == 1 && newdrop->item.count > 6) newdrop->item.count = 6; //limit the drop rate of items from level 1 event mobs
        if(newdrop->item.count==0)
            newdrop->item.count = 1;
        if(newdrop->item.itemtype == 10)
        {
            if(newdrop->item.itemnum >=441 && newdrop->item.itemnum <= 888)// skillbooks
                newdrop->item.count = 1;
        }
        if(newdrop->item.itemtype == 11) //gems only get 1
            newdrop->item.count = 1;
        if(newdrop->item.itemtype == 12)
        {
            if(newdrop->item.itemnum > 300 && newdrop->item.itemnum < 360) //bullets get a lot higher count.
            {
                newdrop->item.count *= 10;
                newdrop->item.count += 10;
            }
        }
    }
    else if( newdrop->item.itemtype >1 && newdrop->item.itemtype !=7 && newdrop->item.itemtype < 10)
    {
        // check to see if the item will be refined
        int prefine = rand()%100; //Probability of finding a refined item
        int refinechance = Config.RefineChance;
        if(prefine < refinechance) // default = 5%
        {
            int refinelevel = rand()%101;  //which level of refine do we actually get
            if( refinelevel < 5)        //default 5%
                newdrop->item.refine = 4 * 16;
            else if( refinelevel < 15 )   //10%
                newdrop->item.refine = 3 * 16;
            else if(refinelevel < 35 )   // 20%
                newdrop->item.refine = 2 * 16;
            else                          // 65%
                newdrop->item.refine = 16;
        }
        else //99%
            newdrop->item.refine = 0;

        // will the item be a blue?
        bool blue = false;
        int bluechance1 = RandNumber( 1, 100);
        int bluechance2 = Config.BlueChance + chamod;
        Log( MSG_INFO, "Blue chance = %i", bluechance2);
        //This probability is now configurable from WorldServer.conf. CHA also has an effect
        if(bluechance1 < bluechance2) // some percentage of drops will be specials or blues whenever one is available.
        {
            Log( MSG_INFO, "Selected a blue item");
            int p = 1;
            while(alternate[n][p] != 0 && p < 8)
            {
                p++;
            }
            if(p > 1) // blues available for this item
            {
                //Log( MSG_INFO, "blue item available");
                p--;
                int bluenum = RandNumber( 1, p);
                newdrop->item.itemnum = alternate[n][bluenum];
                blue=true;
            }
        }
        // will the items get stats? All blue items will. Uniques count as blues.
        int pstats = rand()%101; //Probability of the item having stats. default = 5%
        if(blue == true)
        {
            pstats = 1;
        }
        int StatChance = Config.StatChance;
        if( pstats < StatChance)        // default 5%
            newdrop->item.stats = RandNumber( 1, 300);
    }
    newdrop->item.gem = 0;
    return newdrop;
}


UINT CWorldServer::GetColorExp( UINT playerlevel,UINT moblevel, UINT exp )
{
/*
purple = +16 - 150%
red = +10 - 120%
brown = +4 - 110%
orange = same - 100%
green = -4 - 80%
blue = -9 - 50%
whie = -15 - 20%
*/
    int dif = moblevel - playerlevel;
    if(dif>15)//purple
        exp = (UINT)floor(exp*1.5);
    else if(dif>10)
        exp = (UINT)floor(exp*1.2);
    else if(dif>4)
        exp = (UINT)floor(exp*1.1);
    else if(dif>-4)
        exp = (UINT)floor((double)(exp*1));
    else if(dif>-9)
        exp = (UINT)floor(exp*0.8);
    else if(dif>-15)
        exp = (UINT)floor(exp*0.5);
    else
        exp = (UINT)floor(exp*0.2);
    return exp;
}

bool CWorldServer::SendSysMsg( CPlayer* thisclient, string message )
{
	BEGINPACKET( pak, 0x7df );
	ADDBYTE    ( pak, 0xf1 );
	ADDSTRING  ( pak, message.c_str( ));
	ADDBYTE    ( pak, 0x00 );
	thisclient->client->SendPacket( &pak );
    return true;
}


// teleport to specificated map and position
bool CWorldServer::TeleportTo ( CPlayer* thisclient, int map, fPoint position )
{
    MapList.Index[map]->TeleportPlayer( thisclient, position, false );
    return true;
}


// Learn Skill
bool CWorldServer::LearnSkill( CPlayer* thisclient, UINT skill, bool cost )
{
/*
0 - ya aprendido
1 - aprendio
2 - no job
3 - falta otro skill
4 - no status points (lvl)
5 - no se puede subir de lvl el skill
6 - numero incorrecto de skill
7 - no tienes suficientes sp
*/
        BEGINPACKET(pak1, 0 );
        BEGINPACKET(pak2, 0 );
        BEGINPACKET(pak3, 0 );
        //int b=1;
        CSkills* thisskill = SkillList.at( skill);
	    char skilltype = thisskill->type;
	    //int skillpoints = thisskill->sp;
	    int newslot = thisclient->GetNewSkillSlot(skilltype);
	    //CSkills* thisskill = 0;
	    if (newslot != 0xffff)
	    {
		switch(skilltype)
		{
		case 0:
			thisclient->bskills[newslot]= skill;
			//thisskill = GetSkillByID(thisclient->bskills[newslot]);
			if (cost)
            	thisclient->CharInfo->SkillPoints-=thisskill->sp;
			RESETPACKET(pak1, 0x7b0);
			ADDBYTE(pak1, 1);
			ADDBYTE(pak1, newslot);
			ADDWORD(pak1, skill);
			ADDWORD(pak1, thisclient->CharInfo->SkillPoints);
			thisclient->client->SendPacket(&pak1);
			thisclient->SetStats();
			return true;
		case 1:
			thisclient->askill[newslot]= skill;
			//thisskill = GetSkillByID( (thisclient->askill[newslot])+(thisclient->askilllvl[newslot]));
			if (cost)
            	thisclient->CharInfo->SkillPoints-=thisskill->sp;
			RESETPACKET(pak2, 0x7b0);
			ADDBYTE(pak2, 1);
			ADDBYTE(pak2, 30+newslot);
			ADDWORD(pak2, skill);
			ADDWORD(pak2, thisclient->CharInfo->SkillPoints);
			thisclient->client->SendPacket(&pak2);
			thisclient->SetStats();
			return true;
		case 2:
			thisclient->pskill[newslot]= skill;
			//thisskill = GetSkillByID( (thisclient->pskill[newslot])+(thisclient->pskilllvl[newslot]));
			if (cost)
            	thisclient->CharInfo->SkillPoints-=thisskill->sp;
			RESETPACKET(pak3, 0x7b0);
			ADDBYTE(pak3, 1);
			ADDBYTE(pak3, 60+newslot);
			ADDWORD(pak3, skill);
			ADDWORD(pak3, thisclient->CharInfo->SkillPoints);
			thisclient->client->SendPacket(&pak3);
			thisclient->SetStats();
			return true;
		}
	}
	return false;
}

//add / remove a Fairy
void CWorldServer::DoFairyStuff(CPlayer* targetclient, int action)
{    //irose has no fairys as far as i know
 /*   BEGINPACKET( pak, 0x7dc );
	ADDBYTE( pak, action );
	ADDWORD( pak, targetclient->clientid);
	SendToVisible( &pak, targetclient );*/
}

void CWorldServer::DoFairyFree( int fairy )
{
    GServer->FairyList.at(fairy)->assigned = false;
    GServer->FairyList.at(fairy)->ListIndex = 0;

    if(GServer->Config.FairyTestMode == 0)
         GServer->FairyList.at(fairy)->WaitTime = GServer->Config.FairyWait * (rand()% GServer->GetFairyRange(1)+ GServer->GetFairyRange(0));

    GServer->FairyList.at(fairy)->LastTime = clock();
}

void CWorldServer::RefreshFairy( )
{
        //fairy: The wait timer should be possibibly bigger when there is few people online.
        if (Config.FairyMode == 1 && ClientList.size() > 1){   //if fairy mode on and someone online
           for (int i=0; i<Config.FairyMax; i++)     // check all fairies
           {
                if ( FairyList.at(i)->LastTime <= ( clock() - (FairyList.at(i)->WaitTime*60*CLOCKS_PER_SEC) ) && !FairyList.at(i)->assigned )  // if fairy hour is now
	            {
                    if(ClientList.size() >= 5) //stop solo players fromgetting fairied and screwing up their stats during testing.
                    {
                    int value = rand()%(int)(ClientList.size()-1) + 1;  //choose random value in clientlist
		            CPlayer* targetclient = (CPlayer*) ClientList.at(value)->player;
                    FairyList.at(i)->ListIndex = value;
			        FairyList.at(i)->LastTime = clock();
			        FairyList.at(i)->assigned = true;
			        targetclient->Fairy = true;
			        targetclient->FairyListIndex = i;                 // FairyList index number of our actual fairy
			        DoFairyStuff(targetclient, 1);           // spawn fairy to target
			        targetclient->SetStats();
                    }
                }
			    if ( (FairyList.at(i)->LastTime + (Config.FairyStay*60*CLOCKS_PER_SEC)) <= clock() && FairyList.at(i)->assigned)  // id time for our fairy to go away
    			{
                    CPlayer* oldclient  = (CPlayer*) ClientList.at(FairyList.at(i)->ListIndex)->player;
                    DoFairyFree(i);
                    oldclient->Fairy = false;
                    oldclient->FairyListIndex = 0;
                    DoFairyStuff(oldclient, 0);  // unspawn fairy
                    oldclient->SetStats();
                }
           }
        }
        // this close fairies after their time if GM de activate  fairy mode when some fairies are assigned.
        if (Config.FairyMode == 0 && ClientList.size() > 1){    // if serevr mode off and someone online
             for (int i=0; i<Config.FairyMax; i++)
             {
                 if ( (FairyList.at(i)->LastTime + (Config.FairyStay*60*CLOCKS_PER_SEC)) <= clock() && FairyList.at(i)->assigned)
    			{
                    CPlayer* oldclient  = (CPlayer*) ClientList.at(FairyList.at(i)->ListIndex)->player;
                    DoFairyFree(i);
                    oldclient->Fairy = 0;
                    oldclient->FairyListIndex = 0;
                    DoFairyStuff(oldclient, 0);
                    oldclient->SetStats();
                }
             }
         }
}
