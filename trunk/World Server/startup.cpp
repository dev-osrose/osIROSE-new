/*
    Rose Online Server Emulator
    Copyright(C) 2006,2007 OSRose Team http://www.osrose.net

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or(at your option) any later version.

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
bool CWorldServer::LoadSTBData()
{
    Log(MSG_LOAD, "STB Data             ");
    STBStoreData("3DData\\STB\\LIST_NPC.STB", &STB_NPC);
    STBStoreData("3DData\\STB\\LIST_SKILL.STB", &STB_SKILL);
    STBStoreData("3DData\\STB\\LIST_STATUS.STB", &STB_STATUS);
    STBStoreData("3DData\\STB\\LIST_QUEST.STB", &STB_QUEST);
    STBStoreData("3DData\\STB\\LIST_FACEITEM.STB", &STB_ITEM[0]);
	STBStoreData("3DData\\STB\\LIST_CAP.STB", &STB_ITEM[1]);
	STBStoreData("3DData\\STB\\LIST_BODY.STB", &STB_ITEM[2]);
	STBStoreData("3DData\\STB\\LIST_ARMS.STB", &STB_ITEM[3]);
	STBStoreData("3DData\\STB\\LIST_FOOT.STB", &STB_ITEM[4]);
	STBStoreData("3DData\\STB\\LIST_BACK.STB", &STB_ITEM[5]);
	STBStoreData("3DData\\STB\\LIST_JEWEL.STB", &STB_ITEM[6]);
	STBStoreData("3DData\\STB\\LIST_WEAPON.STB", &STB_ITEM[7]);
	STBStoreData("3DData\\STB\\LIST_SUBWPN.STB", &STB_ITEM[8]);
	STBStoreData("3DData\\STB\\LIST_USEITEM.STB", &STB_ITEM[9]);
	STBStoreData("3DData\\STB\\LIST_JEMITEM.STB", &STB_ITEM[10]);
	STBStoreData("3DData\\STB\\LIST_NATURAL.STB", &STB_ITEM[11]);
	STBStoreData("3DData\\STB\\LIST_QUESTITEM.STB", &STB_ITEM[12]);
	STBStoreData("3DData\\STB\\LIST_PAT.STB", &STB_ITEM[13]);
	STBStoreData("3DData\\STB\\LIST_PRODUCT.STB", &STB_PRODUCT);
	STBStoreData("3DData\\STB\\LIST_SELL.STB", &STB_SELL);
	STBStoreData("3DData\\STB\\LIST_ZONE.STB", &STB_ZONE);
	STBStoreData("3DData\\STB\\ITEM_DROP.STB", &STB_DROP);
}
bool CWorldServer::LoadNPCData()
{
	Log(MSG_LOAD, "NPC Data           ");
    for(unsigned int i = 0; i<STB_NPC.rowcount;i++)
    {
        CNPCData* newnpc = new (nothrow) CNPCData;
        if(newnpc == NULL)
        {
            Log(MSG_ERROR, "Error allocing memory");
            continue;
        }
        newnpc->id = i;
        newnpc->life = STB_NPC.rows[i][1];
        newnpc->stance = mRUNNING;
        newnpc->wspeed = STB_NPC.rows[i][2];
        newnpc->rspeed = STB_NPC.rows[i][3];
        newnpc->dspeed = STB_NPC.rows[i][4];
        newnpc->weapon = STB_NPC.rows[i][5];
        newnpc->subweapon = STB_NPC.rows[i][6];
        newnpc->level = STB_NPC.rows[i][7];
        newnpc->hp = STB_NPC.rows[i][8];
        newnpc->atkpower = STB_NPC.rows[i][9];
        newnpc->hitrate = STB_NPC.rows[i][10];
        newnpc->defense = STB_NPC.rows[i][11];
        newnpc->magicdefense = STB_NPC.rows[i][12];
        newnpc->dodge = STB_NPC.rows[i][13];
        newnpc->atkspeed = STB_NPC.rows[i][14];
        newnpc->magicattack = STB_NPC.rows[i][15];
        newnpc->AI = STB_NPC.rows[i][16];
        newnpc->exp = STB_NPC.rows[i][17];
        newnpc->dropid = STB_NPC.rows[i][18];
        newnpc->money = STB_NPC.rows[i][19];
        newnpc->item = STB_NPC.rows[i][20];
        newnpc->tab1 = STB_NPC.rows[i][21];
        newnpc->tab2 = STB_NPC.rows[i][22];
        newnpc->tab3 = STB_NPC.rows[i][23];
        newnpc->specialtab = STB_NPC.rows[i][24];
        newnpc->atkdistance = STB_NPC.rows[i][26]/100;
        newnpc->aggresive = STB_NPC.rows[i][27];
        newnpc->shp = STB_NPC.rows[i][42];
        newnpc->dialogid = 0;
        NPCData.push_back(newnpc);
    }
	return true;
}

bool CWorldServer::LoadStatusData()
{
    Log(MSG_LOAD, "Status Data             ");
    for(unsigned int i = 0; i < STB_STATUS.rowcount; i++)
    {
        CStatus* newstatus = new(nothrow) CStatus;
        if(newstatus == NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: status_data");
            return false;
        }
        newstatus->id = i;
        newstatus->type = STB_STATUS.rows[i][1];  // Unk
        newstatus->duplication = STB_STATUS.rows[i][2]; // Unk
        newstatus->dir = STB_STATUS.rows[i][3]; // 0: Up 1: Down 2: ?
        newstatus->repeat = STB_STATUS.rows[i][4]; // 1: Repeat(Heal) 2: Once(Buff Stat) 3: Status Effect(Poison, etc)
        newstatus->ability[0] = STB_STATUS.rows[i][5]; // Status # to call?
        newstatus->amount[0] = STB_STATUS.rows[i][6]; // Amount
        newstatus->ability[1] = STB_STATUS.rows[i][7];
        newstatus->amount[1] = STB_STATUS.rows[i][8];
        newstatus->decrease = STB_STATUS.rows[i][17]; // 0: Increase 1: Decrease

        StatusList.push_back(newstatus);
    }
    return true;
}

bool CWorldServer::LoadSkillData()
{
	Log(MSG_LOAD, "Skills data            ");
       for(unsigned int i = 0; i<STB_SKILL.rowcount;i++)
    {
        CSkills* newskill = new(nothrow) CSkills;
        if(newskill == NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: skills_data");
            return false;
        }
        char *tmp;
        newskill->id=i;
        newskill->level=STB_SKILL.rows[i][2];  // Skills Level
        newskill->sp=STB_SKILL.rows[i][3];     // Cost to get skill
        newskill->type=STB_SKILL.rows[i][4];   //
        newskill->skilltype=STB_SKILL.rows[i][5]; // Type of skill
        newskill->range=STB_SKILL.rows[i][6]/100; // Range of skill
        newskill->target=STB_SKILL.rows[i][7];    // Skill Target
        newskill->aoerange=STB_SKILL.rows[i][8]/100; // AOE Range
        newskill->atkpower=STB_SKILL.rows[i][9];     // Attack Power
        newskill->status[0]=STB_SKILL.rows[i][11];  // stat
        newskill->status[1]=STB_SKILL.rows[i][12];  // stat
        newskill->success=STB_SKILL.rows[i][13];    // Success Rate
        if(newskill->success == 0)
            newskill->success = 100;                //success rate is stored as NULL where it is 100%. eg dual scratch
        newskill->duration = STB_SKILL.rows[i][14];   // Duration
        newskill->costtype[0] = STB_SKILL.rows[i][16];   //not all costs are in MP
        newskill->costamount[0] = STB_SKILL.rows[i][17]; //some are in HP or Stamina
        newskill->mp = STB_SKILL.rows[i][17];
        newskill->costtype[1] = STB_SKILL.rows[i][18]; // All driveby skills have 2 costs
        newskill->costamount[1] = STB_SKILL.rows[i][19]; // The second is normally cart gauge
        newskill->cooldown = STB_SKILL.rows[i][20];   // The time it takes to be able to use the skill again.
        //newskill->success=STB_SKILL.rows[i][69];    //Dunno what this field is really but it sure isn't success rate <_<
        newskill->weapon[0] = STB_SKILL.rows[i][30];    // Item Type must be equipped(Only 1)
        newskill->weapon[1] = STB_SKILL.rows[i][31];    // Item Type must be equipped
        newskill->weapon[2] = STB_SKILL.rows[i][32];    // Item Type must be equipped
        newskill->weapon[3] = STB_SKILL.rows[i][33];    // Item Type must be equipped
        newskill->weapon[4] = STB_SKILL.rows[i][34];    // Item Type must be equipped
        newskill->c_class[0] = STB_SKILL.rows[i][35];   // Required Class(Only One)
        newskill->c_class[1] = STB_SKILL.rows[i][36];   // Required Class
        newskill->c_class[2] = STB_SKILL.rows[i][37];   // Required Class
        newskill->c_class[3] = STB_SKILL.rows[i][38];   // Required Class
        newskill->c_class[4] = 0;
        newskill->rskill[0]=STB_SKILL.rows[i][39];    // Required Skill
        newskill->rskill[1]=STB_SKILL.rows[i][41];    // Required Skill
        newskill->rskill[2]=STB_SKILL.rows[i][43];    // Required Skill
        newskill->lskill[0]=STB_SKILL.rows[i][40];    // Required Skill Level
        newskill->lskill[1]=STB_SKILL.rows[i][42];    // Required Skill Level
        newskill->lskill[2]=STB_SKILL.rows[i][44];    // Required Skill Level

        newskill->buff[0]=STB_SKILL.rows[i][21];      // Status
        newskill->value1[0]=STB_SKILL.rows[i][22];    // Int Value
        newskill->value2[0]=STB_SKILL.rows[i][23];    // % Value
        newskill->buff[1]=STB_SKILL.rows[i][24];      // Status
        newskill->value1[1]=STB_SKILL.rows[i][25];    // Int Value
        newskill->value2[1]=STB_SKILL.rows[i][26];    // % Value
        newskill->buff[2]=0;STB_SKILL.rows[i][11];//for damage support
        newskill->value1[2]=0;STB_SKILL.rows[i][9];//for damage support
        newskill->value2[2]=0;

        newskill->req[0]=STB_SKILL.rows[i][45];    //the requirement type(usually 31 = level)
        newskill->clevel=STB_SKILL.rows[i][46];    //requirement amount
        newskill->reqam[0]=STB_SKILL.rows[i][46];  //this need no always be level but is usually
        newskill->req[1]=STB_SKILL.rows[i][47];    //the requirement type(in most cases blank)
        newskill->reqam[1]=STB_SKILL.rows[i][48];  //requirement amount
        newskill->zuly =(STB_SKILL.rows[i][85] * 100);    // Required zuly

        // I think any skill with an AOE radius is indeed AOE, it's just not all do damage
        if((STB_SKILL.rows[i][8])>0) //this boolean is kind of pointless too. Not all skills with AOE range are actually designated as AOE. We should use skilltype instead.
        {
            newskill->aoe=1;
            newskill->aoeradius=STB_SKILL.rows[i][8]/100;
        }
        else
        {
            newskill->aoe=0;
            newskill->aoeradius=0;
        }
        if((STB_SKILL.rows[i][28])>0)
        {
           newskill->script=1;
           newskill->svalue1=STB_SKILL.rows[i][28];
        }
        else
        {
           newskill->script=0;
           newskill->svalue1=0;
        }
        newskill->nbuffs = 0;
        if(newskill->buff[0]!=0)
            newskill->nbuffs += 1;
        if(newskill->buff[1]!=0)
            newskill->nbuffs += 2;
        if(newskill->buff[2]!=0)
            newskill->nbuffs += 4;

        SkillList.push_back(newskill);
	}
    return true;
}

bool CWorldServer::LoadTeleGateData()
{
	Log(MSG_LOAD, "Telegates data       ");
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT `id`, `srcx`, `srcy`, `srcmap`, `destx`, `desty`, `destmap` FROM `list_telegates`");
	if(result == NULL)
	{
	    return false;
	}
	while(row = mysql_fetch_row(result))
    {
		CTeleGate* thisgate = new(nothrow) CTeleGate;
        if(thisgate == NULL)
        {
            Log(MSG_ERROR, "Error allocing memory       ");
            DB->QFree();
            return false;
        }
		thisgate->id = atoi(row[0]);
		thisgate->src.x = (float)atof(row[1]);
		thisgate->src.y = (float)atof(row[2]);
		thisgate->srcMap = atoi(row[3]);
		thisgate->dest.x = (float)atof(row[4]);
		thisgate->dest.y = (float)atof(row[5]);
		thisgate->destMap = atoi(row[6]);
		TeleGateList.push_back(thisgate);
	}
	DB->QFree();
	return true;
}

bool CWorldServer::LoadRespawnData()
{
	Log(MSG_LOAD, "RespawnZones data        ");
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT `id`,`x`,`y`,`map`,`radius`,`type` FROM `list_respawnzones`");
    if(result==NULL) return false;
	while(row = mysql_fetch_row(result))
    {
		CRespawnPoint* thisrespawnpoint = new(nothrow) CRespawnPoint;
        if(thisrespawnpoint==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory");
            DB->QFree();
            return false;
        }
		thisrespawnpoint->id = atoi(row[0]);
		thisrespawnpoint->dest.x =(float)atof(row[1]);
		thisrespawnpoint->dest.y =(float)atof(row[2]);
		thisrespawnpoint->destMap = atoi(row[3]);
		thisrespawnpoint->radius = atoi(row[4]);
		thisrespawnpoint->masterdest =(atoi(row[5]) == 1);
		MapList.Index[thisrespawnpoint->destMap]->RespawnList.push_back(thisrespawnpoint);
	}
	DB->QFree();
	return true;
}

#ifdef USEIFO
bool CWorldServer::LoadMobGroups()
{
    Log(MSG_LOAD, "MobGroups data    ");
    vector<CMobGroup*> mobGroups;
    MYSQL_ROW row;
    bool flag = true;
    char* tmp = NULL;
    MYSQL_RES *result = DB->QStore("SELECT `id`, `map`, `x`, `y`, `range`, `respawntime`, `limit`, `tacticalpoints`, `moblist` FROM `list_mobgroups`");
    if(result == NULL) return false;
    while(row = mysql_fetch_row(result))
    {
        CMobGroup* thisgroup = new(nothrow) CMobGroup;
        if(thisgroup == NULL)
        {
            Log(MSG_ERROR, "Error allocating memory");
            DB->QFree();
            return false;
        }
        thisgroup->id = atoi(row[0]);
        thisgroup->map = atoi(row[1]);
        thisgroup->point.x = atof(row[2]);
        thisgroup->point.y = atof(row[3]);
        thisgroup->range = atoi(row[4]);
        thisgroup->respawntime = atoi(row[5]);
        thisgroup->limit = atoi(row[6]);
        thisgroup->tacticalpoints = atoi(row[7]);
        char* mobList = row[8];

        thisgroup->lastRespawnTime = clock();
        thisgroup->active = 0;
        thisgroup->basicKills = 0;
        thisgroup->curTac = 0;
        thisgroup->curBasic = 0;

        thisgroup->basicMobs.clear();
        thisgroup->tacMobs.clear();

        // Fill in basic/tac mobs
        tmp = strtok(mobList, ",|");
        while(tmp != NULL)
        {
            int mobId = atoi(tmp);
            tmp = strtok(NULL, ",|");
            if(tmp == NULL)
            {
                Log(MSG_ERROR, "MobGroup %i is invalid", thisgroup->id);
                flag = false;
                break;
            }
            int amount = atoi(tmp);
            tmp = strtok(NULL, ",|");
            if(tmp == NULL)
            {
                Log(MSG_ERROR, "MobGroup %i is invalid", thisgroup->id);
                flag = false;
                break;
            }
            int tactical = atoi(tmp);
            CMob *thismob = new(nothrow) CMob;
            if(thismob == NULL)
            {
                Log(MSG_ERROR, "Error allocating memory");
                DB->QFree();
                return false;
            }
            thismob->amount = amount;
            thismob->tactical = tactical;
            thismob->mobId = mobId;
            thismob->thisnpc = GetNPCDataByID(thismob->mobId);
            thismob->mapdrop = GetDropData(thisgroup->map);
            thismob->mobdrop = GetDropData(thismob->thisnpc->dropid);
            if(thismob->thisnpc == NULL) {
                Log(MSG_WARNING, "Invalid mobId - Mob %i will not be added", atoi(row[0]));
                delete thismob;
                continue;
            }
            if(thismob->tactical)
            {
                thisgroup->tacMobs.push_back(thismob);
            }
            else
            {
                thisgroup->basicMobs.push_back(thismob);
            }
            tmp = strtok(NULL, ",|");
        }
        if(!flag)
        {
            delete thisgroup;
            continue;
        }
        MapList.Index[thisgroup->map]->MobGroupList.push_back(thisgroup);
        mobGroups.push_back(thisgroup);
    }
	DB->QFree();
	return true;
}
#endif

bool CWorldServer::LoadMonsterSpawn()
{
	Log(MSG_LOAD, "SpawnZones data      ");
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT `id`,`map`,`montype`,`min,max`,`respawntime`,`points` FROM `list_spawnareas`");
	if(result==NULL) return false;
	while(row = mysql_fetch_row(result))
    {
        bool flag = true;
		char* tmp;
		CSpawnArea* thisspawn = new(nothrow) CSpawnArea;
        if(thisspawn==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory");
            DB->QFree();
            return false;
        }
		thisspawn->id=atoi(row[0]);
		thisspawn->map=atoi(row[1]);
		thisspawn->montype=atoi(row[2]);
		thisspawn->min=0;//atoi(row[3]);
		thisspawn->max=atoi(row[4]);
		thisspawn->respawntime=atoi(row[5]);
		if(thisspawn->respawntime==0)thisspawn->respawntime=5;
		thisspawn->amon = 0;
		char* points;
		points = row[6];
		thisspawn->pcount = atoi(strtok(points , ",|"));
		thisspawn->points = new(nothrow) fPoint[thisspawn->pcount];
		if(thisspawn->points==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory       ");
            delete thisspawn;
            DB->QFree();
            return false;
        }
		thisspawn->lastRespawnTime = clock();
		for(int i=0; i<thisspawn->pcount; i++)
        {
			if((tmp = strtok(NULL, ",|"))==NULL)
            {
				Log(MSG_ERROR, "Spawn area %i have invalid points",thisspawn->id);
				flag = false;
				break;
			}
			float x=(float)atof(tmp);
			if((tmp = strtok(NULL, ",|"))==NULL)
            {
				Log(MSG_ERROR, "Spawn area %i have invalid points",thisspawn->id);
				flag = false;
				break;
			}
			float y=(float)atof(tmp);
			thisspawn->points[i].x = x;
			thisspawn->points[i].y = y;
		}
        if(flag)
        {
            thisspawn->thisnpc = GetNPCDataByID(thisspawn->montype);
            if(thisspawn->thisnpc==NULL)
            {
                Log(MSG_WARNING, "Invalid montype - Spawn %i will not be added", thisspawn->id);
                delete thisspawn;
                continue;
            }
    		MapList.Index[thisspawn->map]->MonsterSpawnList.push_back(thisspawn);
        }
	}
	DB->QFree();
	return true;
}

bool CWorldServer::LoadNPCs()
{
	Log(MSG_LOAD, "NPC spawn        ");
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT `type`,`map`,`dir`,`x`,`y`,`dialogid` FROM `list_npcs`");
	if(result==NULL) return false;
	while(row = mysql_fetch_row(result))
    {
		CNPC* thisnpc = new(nothrow) CNPC;
        if(thisnpc==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory");
            DB->QFree();
            return false;
        }
		thisnpc->clientid = GetNewClientID();
		thisnpc->npctype = atoi(row[0]);
		thisnpc->posMap = atoi(row[1]);
		thisnpc->dir =(float)atof(row[2]);
		thisnpc->pos.x =(float)atof(row[3]);
		thisnpc->pos.y =(float)atof(row[4]);
		thisnpc->thisnpc = GetNPCDataByID(thisnpc->npctype);
		if(thisnpc->thisnpc == NULL)
		{
            delete thisnpc;
            continue;
        }
    	thisnpc->thisnpc->dialogid = atoi(row[5]); // This is global to NPC type
		MapList.Index[thisnpc->posMap]->AddNPC(thisnpc);
	}
	DB->QFree();
	return true;
}

bool CWorldServer::LoadDropsData()
{
	Log(MSG_LOAD, "Drops Data       ");
    FILE* fh = NULL;
    fh = fopen("data/drops_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/drops_data.csv");
        return false;
    }
    char line[500];
    fgets(line, 500, fh);// this is the column name
    while(!feof(fh))
    {
        memset(&line, '\0', 500);
        fgets(line, 500, fh);
        CMDrops* newdrop = new(nothrow) CMDrops;
        if(newdrop == NULL)
        {
            fclose(fh);
            continue;
        }
        newdrop->id = GetUIntValue(",", &line);
        char* items = GetStrValue(",");
        char* prob = GetStrValue(",");
        newdrop->level_min = GetUIntValue(",");
        newdrop->level_max = GetUIntValue(",");
        newdrop->level_boss = GetUIntValue(",");
        UINT value = 0;
        bool First = true;
        // items
        while((value = GetUIntValue("|", First?items:NULL))!=0)
        {
            First = false;
            CDropInfo* DropInfo = new(nothrow) CDropInfo;
            if(DropInfo == NULL)
            {
                Log(MSG_WARNING, "\nError allocing memory [dropinfo]");
                continue;
            }
            if(value < 20000)
            {
                DropInfo->type = value / 1000;
                DropInfo->item = value % 1000;
            }
            else
            {
                DropInfo->type = value / 1000000;
                DropInfo->item = value % 1000000;
            }
            newdrop->Drops.push_back(DropInfo);
        }
        newdrop->probmax = 0;
        value = 0;
        // probability
        for(UINT j=0;j<newdrop->Drops.size();j++)
        {
            value = GetUIntValue("|",(j==0?prob:NULL));
            if(value == 0)
            {
                newdrop->Drops.at(j)->prob = 1;
                if(newdrop->Drops.at(j)->type < 10 || newdrop->Drops.at(j)->type == 14)
                {
                    newdrop->Drops.at(j)->prob *= Config.DROP_RATE;
                }
                Log(MSG_WARNING, "Probability is not complete, dropid: %i - temporal probability will be 1 * rate", newdrop->id);
            }
            else
            {
                newdrop->Drops.at(j)->prob = value;
                if(newdrop->Drops.at(j)->type < 10 || newdrop->Drops.at(j)->type == 14)
                {
                    newdrop->Drops.at(j)->prob *= Config.DROP_RATE;
                }
            }
            newdrop->probmax += newdrop->Drops.at(j)->prob;
        }
        // sort time
        for(UINT j=0;j<newdrop->Drops.size();j++)
        {
            for(UINT k=j;k<newdrop->Drops.size();k++)
            {
                if(newdrop->Drops.at(j)>newdrop->Drops.at(k))
                {
                    CDropInfo* DropInfo = newdrop->Drops.at(j);
                    newdrop->Drops.at(j) = newdrop->Drops.at(k);
                    newdrop->Drops.at(k) = DropInfo;
                }
            }
        }
        MDropList.push_back(newdrop);
	}
	fclose(fh);
	return true;
}


bool CWorldServer::LoadPYDropsData()
{
    Log(MSG_INFO, "Loading PYDrops Data");
    MDropList.clear();
    MYSQL_ROW row;
    MYSQL_RES *result = DB->QStore("SELECT `id`,`type`,`min_level`,`max_level`,`prob`,`mob`,`map`,`alt` FROM `item_drops`");
    if(result == NULL)
    {
        DB->QFree();
        return false;
    }
    while(row = mysql_fetch_row(result))
    {
        CMDrops* newdrop = new(nothrow) CMDrops;
        assert(newdrop);
        newdrop->itemnum = atoi(row[0]);
        newdrop->itemtype = atoi(row[1]);
        newdrop->level_min = atoi(row[2]);
        newdrop->level_max = atoi(row[3]);
        newdrop->prob = atoi(row[4]);
        newdrop->mob = atoi(row[5]);
        newdrop->map = atoi(row[6]);
        char *tmp;
        if((tmp = strtok(row[7], "|")) == NULL)
        {
            newdrop->alt[0] = 0;
        }
        else
        {
            newdrop->alt[0] = atoi(tmp);
        }
        for(unsigned int i=1;i<8; i++)
        {
            if((tmp = strtok(NULL, "|")) == NULL)
            {
                newdrop->alt[i] = 0;
            }
            else
            {
                newdrop->alt[i] = atoi(tmp);
            }
        }
        //if(newdrop->mob != 0)
            //Log(MSG_INFO, "found a non zero mob value %i",newdrop->mob);
        MDropList.push_back(newdrop);
    }
    DB->QFree();
    Log(MSG_INFO, "PYDrops loaded");
    return true;
}

bool CWorldServer::LoadSkillBookDropsData()
{
    Log(MSG_INFO, "Loading Skillbook data");
    MYSQL_ROW row;
    MYSQL_RES *result = DB->QStore("SELECT `id`,`itemtype`,`min`,`max`,`prob` FROM `list_skillbooks`");
    if(result==NULL)
    {
        DB->QFree();
        return false;
    }
    int c = 0;
    while(row = mysql_fetch_row(result))
    {

        c++;
        CMDrops* newdrop = new(nothrow) CMDrops;
        assert(newdrop);
        newdrop->itemnum = atoi(row[0]);
        newdrop->itemtype = atoi(row[1]);
        newdrop->level_min = atoi(row[2]);
        newdrop->level_max = atoi(row[3]);
        newdrop->prob = atoi(row[4]);
        SkillbookList.push_back(newdrop);
    }
    DB->QFree();
    Log(MSG_INFO, "Skillbook Data loaded");
    return true;
}

bool CWorldServer::LoadConfig()
{
    Log(MSG_INFO, "Loading database config");
    MYSQL_ROW row;
    MYSQL_RES *result = DB->QStore("SELECT `exp_rate`, `drop_rate`, `zuly_rate`, `blue_chance`, `stat_chance`, `slot_chance`, \
        `refine_chance`, `rare_refine`, `kill_on_fail`, `player_damage`, `monster_damage`, `player_acc`, `monster_acc`, \
        `pvp_acc`, `skill_damage` FROM `list_config`");
    if(result==NULL)
    {
        DB->QFree();
        return false;
    }
    if(mysql_num_rows(result) == 0) {
      DB->QFree();
      return false;
    }
    while(row = mysql_fetch_row(result))
    {
       GServer->Config.EXP_RATE = atoi(row[0]);
       GServer->Config.DROP_RATE = atoi(row[1]);
       GServer->Config.ZULY_RATE = atoi(row[2]);
       GServer->Config.BlueChance = atoi(row[3]);
       GServer->Config.StatChance = atoi(row[4]);
       GServer->Config.SlotChance = atoi(row[5]);
       GServer->Config.RefineChance = atoi(row[6]);
       GServer->Config.Rare_Refine = atoi(row[7]);
       GServer->Config.KillOnFail = atoi(row[8]);
       GServer->Config.PlayerDmg = atoi(row[9]);
       GServer->Config.MonsterDmg = atoi(row[10]);
       // Not implemented in server yet - Drakia
       //GServer->Config.PlayerAcc = atoi(row[11]);
       //GServer->Config.MonsterAcc = atoi(row[12]);
       //GServer->Config.PvpAcc = atoi(row[13]);
       //GServer->Config.SkillDmg = atoi(row[14]);
    }
    DB->QFree();
    Log(MSG_INFO, "Config Data Loaded");
    return true;
}


bool CWorldServer::LoadMonsters()
{
	Log(MSG_LOAD, "Monsters Spawn       ");
	// Do our monster spawnin
#ifndef USEIFO
    for(UINT i=0;i<MapList.Map.size();i++)
    {
        CMap* thismap = MapList.Map.at(i);
        for(UINT j=0;j<thismap->MonsterSpawnList.size();j++)
        {
            CSpawnArea* thisspawn = thismap->MonsterSpawnList.at(j);
    		thisspawn->mapdrop = GetDropData(thisspawn->map);
            thisspawn->mobdrop = GetDropData(thisspawn->thisnpc->dropid);
            for(UINT k=0;k<thisspawn->max;k++)
            {
                fPoint position = RandInPoly(thisspawn->points, thisspawn->pcount);
                thismap->AddMonster( thisspawn->montype, position, 0, thisspawn->mobdrop, thisspawn->mapdrop, thisspawn->id);
            }
        }
    }
#else
    for(UINT i = 0; i < MapList.Map.size(); i++) {
      CMap *thismap = MapList.Map.at(i);
      for(UINT j = 0; j < thismap->MobGroupList.size(); j++) {
        CMobGroup* thisgroup = thismap->MobGroupList.at(j);
        // Load some basic mobs onto map
        for(UINT k = 0; k < thisgroup->limit; k++) {
          CMob* thismob = thisgroup->basicMobs.at(thisgroup->curBasic);
          thisgroup->curBasic++;
          if(thisgroup->curBasic >= thisgroup->basicMobs.size()) thisgroup->curBasic = 0;
          for(UINT l = 0; l < thismob->amount; l++) {
          fPoint position = RandInCircle(thisgroup->point, thisgroup->range);
          thismap->AddMonster(thismob->mobId, position, 0, thismob->mobdrop, thismob->mapdrop, thisgroup->id);
          }
        }
      }
    }
#endif
	return true;
}

bool CWorldServer::LoadUpgrade()//for succes rate
{
	Log(MSG_LOAD, "Refine Data      ");
    FILE* fh = NULL;
    fh = fopen("data/refine_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/refine_data.csv");
        return false;
    }
    char line[50];
    fgets(line, 50, fh);// this is the column name
    while(!feof(fh))
    {
        memset(&line, '\0', 50);
        fgets(line, 50, fh);
        upgrade[GetUIntValue(",",&line)] = GetUIntValue(",");
    }
    fclose(fh);
	return true;
}

bool CWorldServer::CleanConnectedList()
{
    Log(MSG_LOAD, "Cleaning Connected Clients         ");
    DB->QExecute("UPDATE accounts set online=false");
    return true;
}

bool CWorldServer::LoadEquip()
{
    Log(MSG_LOAD, "Equip Data         ");
    for(int j=0;j<9;j++)
    {
        for(unsigned int i=0;i<STB_ITEM[j].rowcount;i++)
        {
            CEquip* newequip = new(nothrow) CEquip;
            if(newequip == NULL)
            {
                Log(MSG_WARNING, "\nError allocing memory: equip");
                return false;
            }
            newequip->id = i;
            newequip->itemtype = (j + 1);
            newequip->equiptype = (j + 1);
            newequip->type = STB_ITEM[j].rows[i][4];
            newequip->price = STB_ITEM[j].rows[i][5];
            newequip->pricerate = STB_ITEM[j].rows[i][6];
            newequip->weight = STB_ITEM[j].rows[i][7];
            newequip->quality = STB_ITEM[j].rows[i][8];
            newequip->level = STB_ITEM[j].rows[i][13];
            newequip->material = STB_ITEM[j].rows[i][14];
            newequip->unionpoint = STB_ITEM[j].rows[i][15];
            newequip->defense = STB_ITEM[j].rows[i][31];
            newequip->magicresistence = STB_ITEM[j].rows[i][32];
            newequip->attackdistance = STB_ITEM[j].rows[i][33]; //Speed of travel/Range
            if(newequip->equiptype == SHOE)
            {
                newequip->movespeed = STB_ITEM[j].rows[i][33];
            }
            else
            {
                newequip->movespeed = 0;
            }

            if(STB_ITEM[j].fieldcount > 35)
            {
                newequip->attackpower = STB_ITEM[j].rows[i][35];
                newequip->attackspeed = STB_ITEM[j].rows[i][36];
                newequip->magicattack = STB_ITEM[j].rows[i][37];//new. Staffs and wands are designated as magic weapons.
                if(newequip->magicattack > 1)
                {
                    newequip->magicattack = 0; //all the blue named spears have weird numbers here. Maybe why NA had a massive damage glitch with them.
                }
            }
            else
            {
                newequip->attackpower = 0;
                newequip->attackspeed = 0;
                newequip->magicattack = 0;
            }


            for(int k=0;k<3;k++)
            {
                newequip->occupation[k] = STB_ITEM[j].rows[i][(16 + k)];
            }
            for(int k=0;k<2;k++)
            {
                newequip->condition1[k] = STB_ITEM[j].rows[i][(19 + k)];
            }
            for(int k=0;k<2;k++)
            {
                newequip->condition2[k] = STB_ITEM[j].rows[i][(21 + k)];
            }
            for(int k=0;k<2;k++)
            {
                newequip->stat1[k] = STB_ITEM[j].rows[i][(24 + k)];
            }
            for(int k=0;k<2;k++)
            {
                newequip->stat2[k] = STB_ITEM[j].rows[i][(27 + k)];
            }
            //newequip->refinetype = STB_ITEM[j].rows[i][45];
            newequip->itemgrade = STB_ITEM[j].rows[i][46];
            newequip->raretype = STB_ITEM[j].rows[i][47];
            if(newequip->id > 4999)
            {
                //Log(MSG_WARNING,"Equipment type %i ID %i is outside of accepted range",newequip->equiptype, newequip->id  );
                //item not added
                //return false;
            }
            else
            {
                EquipList[newequip->equiptype].Data.push_back( newequip );
                EquipList[newequip->equiptype].Index[newequip->id] = newequip; // Index to read more quickly the data
            }
            //delete newequip;
        }
    }
    return true;
}

bool CWorldServer::LoadJemItem()//STB_ITEM[10]
{
    Log(MSG_LOAD, "Jem Data         ");
    for(unsigned int i=0;i<STB_ITEM[10].rowcount;i++)
    {
        CJemData* thisjem = new(nothrow) CJemData;
        if(thisjem == NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: jemitem");
            continue;
        }
        thisjem->id = i;
        thisjem->type = STB_ITEM[10].rows[i][4];
        thisjem->price = STB_ITEM[10].rows[i][5];
        thisjem->pricerate = STB_ITEM[10].rows[i][6];
        thisjem->weight = STB_ITEM[10].rows[i][7];
        thisjem->quality = STB_ITEM[10].rows[i][8];
        thisjem->material = STB_ITEM[10].rows[i][14];
        thisjem->stat1[0] = STB_ITEM[10].rows[i][16];
        thisjem->stat1[1] = STB_ITEM[10].rows[i][17];
        thisjem->stat2[0] = STB_ITEM[10].rows[i][18];
        thisjem->stat2[1] = STB_ITEM[10].rows[i][19];
        JemList.Data.push_back(thisjem);
        JemList.Index[thisjem->id] = thisjem;
    }
    return true;
}

bool CWorldServer::LoadNaturalItem()//STB_ITEM[11]
{
    Log(MSG_LOAD, "Natural Data         ");
    for(unsigned int i=0;i<STB_ITEM[11].rowcount;i++)
    {
        CNaturalData* thisnatural = new(nothrow) CNaturalData;
        if(thisnatural==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: natural");
            return false;
        }
        thisnatural->id = i;
        thisnatural->type = STB_ITEM[11].rows[i][4];
        thisnatural->price = STB_ITEM[11].rows[i][5];
        thisnatural->pricerate = STB_ITEM[11].rows[i][6];
        thisnatural->weight = STB_ITEM[11].rows[i][7];
        thisnatural->quality = STB_ITEM[11].rows[i][8];
        thisnatural->pricevalue = STB_ITEM[11].rows[i][16];
        NaturalList.Data.push_back(thisnatural);
        NaturalList.Index[thisnatural->id] = thisnatural;
    }
    return true;
}

bool CWorldServer::LoadPatItem()
{
    Log(MSG_LOAD, "Consumible Data         ");
    for(unsigned int i=0;i<STB_ITEM[13].rowcount;i++)
    {
        CPatData* newpat = new(nothrow) CPatData;
        if(newpat==NULL)
        {
            Log(MSG_WARNING, "Error allocing memory: pat\n");
            return false;
        }
        newpat->id = i;
        newpat->type = STB_ITEM[13].rows[i][4];
        newpat->price = STB_ITEM[13].rows[i][5];
        newpat->pricerate = STB_ITEM[13].rows[i][6];
        newpat->weight = STB_ITEM[13].rows[i][7];
        newpat->quality = STB_ITEM[13].rows[i][8];
        newpat->material = STB_ITEM[13].rows[i][14];
        newpat->partversion = STB_ITEM[13].rows[i][17];
        newpat->level = STB_ITEM[13].rows[i][22];             // extra field added rev 70
        newpat->condition[1] = STB_ITEM[13].rows[i][24];      // extra field added rev 70
        newpat->modifier[1] = STB_ITEM[13].rows[i][25];       // extra field added rev 70
        newpat->condition[2] = STB_ITEM[13].rows[i][27];      // extra field added rev 70
        newpat->modifier[2] = STB_ITEM[13].rows[i][28];       // extra field added rev 70
        newpat->maxfuel = STB_ITEM[13].rows[i][31];
        newpat->fuelcons = STB_ITEM[13].rows[i][32];
        newpat->speed = STB_ITEM[13].rows[i][33];
        newpat->attackdistance = STB_ITEM[13].rows[i][35];
        newpat->attackpower = STB_ITEM[13].rows[i][36];
        newpat->attackspeed = STB_ITEM[13].rows[i][37];
        PatList.Data.push_back(newpat);
        PatList.Index[newpat->id] = newpat;
    }
    return true;
}

bool CWorldServer::LoadProductItem()//STB_PRODUCT
{
    Log(MSG_LOAD, "Product Data         ");
    for(unsigned int i=0;i<STB_PRODUCT.rowcount;i++)
    {
        CProductData* newproduct = new(nothrow) CProductData;
        if(newproduct == NULL)
        {
            Log(MSG_WARNING, "Error allocing memory: product\n");
            return false;
        }
        newproduct->id = i;
        newproduct->item[0] = STB_PRODUCT.rows[i][1];//material type
        newproduct->amount[0] = STB_PRODUCT.rows[i][3];
        newproduct->item[1] = STB_PRODUCT.rows[i][4];
        newproduct->amount[1] = STB_PRODUCT.rows[i][5];
        newproduct->item[2] = STB_PRODUCT.rows[i][6];
        newproduct->amount[2] = STB_PRODUCT.rows[i][7];
        newproduct->item[3] = STB_PRODUCT.rows[i][8];
        newproduct->amount[3] = STB_PRODUCT.rows[i][9];
        ProductList.Data.push_back(newproduct);
        ProductList.Index[newproduct->id] = newproduct;
    }
    return true;
}

bool CWorldServer::LoadSellData()//STB_SELL
{
    Log(MSG_LOAD, "Sell Data         ");
    for(unsigned int i=0;i<STB_SELL.rowcount;i++)
    {

        CCSellData* newsell = new(nothrow) CCSellData;
        if(newsell==NULL)
        {
            Log(MSG_WARNING, "\nError Allocing memory: sell");
            return false;
        }
        newsell->id = i;
        for(unsigned int j=2;j<STB_SELL.fieldcount;j++)
        {
            newsell->item[j-2] = STB_SELL.rows[i][j];
        }
        SellList.Data.push_back(newsell);
        SellList.Index[newsell->id] = newsell;
    }
    return true;
}

bool CWorldServer::LoadConsItem()
{
    Log(MSG_LOAD, "Consumible Data         ");
    for(unsigned int i=0;i<STB_ITEM[9].rowcount;i++)
    {
        CUseData* newuse = new(nothrow) CUseData;
        if(newuse==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: use");
            return false;
        }
        newuse->id = i;
        newuse->restriction = STB_ITEM[9].rows[i][3];
        newuse->type = STB_ITEM[9].rows[i][4];
        newuse->price = STB_ITEM[9].rows[i][5];
        newuse->pricerate = STB_ITEM[9].rows[i][6];
        newuse->weight = STB_ITEM[9].rows[i][7];
        newuse->quality = STB_ITEM[9].rows[i][8];
        newuse->pricevalue = STB_ITEM[9].rows[i][16];
        newuse->usecondition[0]= STB_ITEM[9].rows[i][17];
        newuse->usecondition[1]= STB_ITEM[9].rows[i][18];
        newuse->useeffect[0]= STB_ITEM[9].rows[i][19];
        newuse->useeffect[1]= STB_ITEM[9].rows[i][20];
        UseList.Data.push_back(newuse);
        UseList.Index[newuse->id] = newuse;
    }
    return true;
}

bool CWorldServer::LoadZoneData()
{
    Log(MSG_LOAD, "Zone Data       ");

   for(unsigned int i=0;i<STB_ZONE.rowcount;i++)
    {
        CMap* newzone = new(nothrow) CMap();
        if(newzone == NULL)
        {
            Log(MSG_WARNING, "Error allocing memory: CMap\n");
            return false;
        }
        newzone->id = i;
        newzone->dungeon =(STB_ZONE.rows[i][4] == 1);
        newzone->dayperiod = STB_ZONE.rows[i][13];
        newzone->morningtime = STB_ZONE.rows[i][14];
        newzone->daytime = STB_ZONE.rows[i][15];
        newzone->eveningtime = STB_ZONE.rows[i][16];
        newzone->nighttime = STB_ZONE.rows[i][17];
        newzone->allowpvp = STB_ZONE.rows[i][18];
        newzone->allowpat = STB_ZONE.rows[i][30]==0?true:false;
        if(i == 20||i == 22)
        {
            newzone->ghost = 0;
        }
        else
        {
            newzone->ghost = 1;
        }
        newzone->MapTime = 0;
        newzone->LastUpdate = clock();
        newzone->CurrentTime = 0;
        newzone->MonsterSpawnList.clear();
#ifdef USEIFO
        newzone->MobGroupList.clear();
#endif
        MapList.Map.push_back(newzone);
        MapList.Index[newzone->id] = newzone;
    }
    return true;
}

bool CWorldServer::LoadItemStats()
{
    Log(MSG_LOAD, "Item Stats         ");
    for(unsigned int i=0;i<STB_ITEM[10].rowcount;i++)
    {
        StatsList[i].stat[0] = STB_ITEM[10].rows[i][16];
        StatsList[i].value[0] = STB_ITEM[10].rows[i][17];
        StatsList[i].stat[1] = STB_ITEM[10].rows[i][18];
        StatsList[i].value[1] = STB_ITEM[10].rows[i][19];
    }
    return true;
}
