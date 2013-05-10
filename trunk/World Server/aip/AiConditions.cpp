// Props to ExJam for this code :D
#include "../WorldServer.h"

//extern CRandomMersenne rg;

int AI_GetAbility(CCharacter* entity, byte btAbType)
{
    switch (btAbType)
    {
    case 0:
        return entity->Stats->Level;
        break;
    case 1:
        return entity->Stats->Attack_Power;
        break;
    case 2:
        return entity->Stats->Defense;
        break;
    case 3:
        return entity->Stats->Magic_Defense;
        break;
    case 4:
        return entity->Stats->HP;
        break;
    case 5:
        return 0;
        //	Log(MSG_WARNING, "Don't have monsters charm yet...");
        //	return -1;
        break;
    default:
        Log(MSG_ERROR, "Unknown cAbType [%d]", btAbType);
        return -1;
    };
}

//Hold or Attack
AICOND(000)
{
    //Always False
    return AI_FAILURE;
}

//(Damage > iDamage) == true iDamage cRecvOrGive
AICOND(001)
{
    GETAICONDDATA(001);
//   Log(MSG_DEBUG, "AIcond(001) %i %i %i",data->iDamage,data->cRecvOrGive,entity->damagecounter);
    if (!data->cRecvOrGive)
    {
        if (entity->damagecounter > data->iDamage)
        {
            entity->damagecounter = 0;
            return AI_SUCCESS;
        }
    }
    else
    {
        if (entity->damagecounter < data->iDamage)
        {
            entity->damagecounter = 0;
            return AI_SUCCESS;
        }
    }
    return AI_FAILURE;

}

//Check Near (1)
AICOND(002)
{
    /*
    dword iDistance;	//Pos: 0x00
    byte btIsAllied;	//Pos: 0x04
    word nLevelDiff;	//Pos: 0x06
    word nLevelDiff2;	//Pos: 0x08
    word wChrNum;	//Pos: 0x0a
    */
    //count how many aiobj between nLevelDiff and nLevelDiff2 who is btIsAllied within iDistance
    //must be >= wChrNum

    GETAICONDDATA(002);
    dword chrCount = 0;
    dword eCount = 0;
    int nearestDistance = 9999999;
    int searchDistance = data->iDistance;
    CMap* map = GServer->MapList.Index[entity->Position->Map];
//	Log(MSG_DEBUG, "Check Near (1)iDistance %i btIsAllied %i nLevelDiff %i nLevelDiff2 %i wChrNum %i", data->iDistance, data->btIsAllied, data->nLevelDiff, data->nLevelDiff2, data->wChrNum);
    if (data->btIsAllied)
    {
        dword entityCount = map->MonsterList.size();
        for (UINT j=0;j<map->MonsterList.size();j++)
        {
            CMonster* other = map->MonsterList.at(j);
            if (eCount >= entityCount) break;
            if (other == NULL) continue;
            eCount++;
            if (other == entity) continue;

            int levelDiff = (entity->Stats->Level - other->Stats->Level);
            if (levelDiff < data->nLevelDiff) continue;
            if (levelDiff > data->nLevelDiff2) continue;
            int iDistance = (int)GServer->distance( other->Position->current, entity->Position->current );
            if (iDistance > searchDistance) continue;

            chrCount++;
            if (iDistance < nearestDistance)
            {
                nearestDistance = iDistance;
                entity->nearChar = other;
            }
            if (chrCount >= data->wChrNum)
            {
                entity->findChar = other;
                return AI_SUCCESS;
            }
        }
    }
    else
    {
        dword entityCount = map->PlayerList.size();
//	 Log(MSG_DEBUG, "entityCount %i", entityCount);
        for (UINT j=0;j<map->PlayerList.size();j++)
        {
            CPlayer* other = map->PlayerList.at(j);
//        Log(MSG_DEBUG, "player %i", other->clientid);
            if (eCount >= entityCount) break;
//        Log(MSG_INFO, "1");
            if (other == NULL) continue;
//        Log(MSG_INFO, "2");
            eCount++;

            int levelDiff = (entity->Stats->Level - other->Stats->Level);
//		Log(MSG_DEBUG, "levelDiff %i < %i", levelDiff,data->nLevelDiff);
            if (levelDiff < data->nLevelDiff) continue;
//		Log(MSG_INFO, "3");
//		Log(MSG_DEBUG, "levelDiff %i > %i", levelDiff,data->nLevelDiff2);
            if (levelDiff > data->nLevelDiff2) continue;
//		Log(MSG_INFO, "4");
            int iDistance = (int)GServer->distance( other->Position->current, entity->Position->current );
            if (iDistance > searchDistance) continue;
//		Log(MSG_INFO, "5");

            chrCount++;
            if (iDistance < nearestDistance)
            {
                nearestDistance = iDistance;
                entity->nearChar = other;
//			Log(MSG_INFO, "6");
            }
            if (chrCount >= data->wChrNum)
            {
                entity->findChar = other;
//			Log(MSG_INFO, "7");
                return AI_SUCCESS;
            }
        }
    }
    return AI_FAILURE;
}

//Check Distance (1)
AICOND(003)
{
    //dword iDistance;	//Pos: 0x00
    //CObjCHAR::Get_MoveDISTANCE > iDistance == true
    //distance from spawn point?
    GETAICONDDATA(003);

    entity->UpdatePosition();
    int moveDistance = (int)GServer->distance( entity->Position->current, entity->Position->source );
    if (moveDistance > (data->iDistance))
    {
        return AI_SUCCESS;
    }
    return AI_FAILURE;
}

//Check Distance (2)
AICOND(004)
{
    //dword iDistance;	//Pos: 0x00
    //byte cMoreLess;	//Pos: 0x04
    //distance to target (cMoreLess == 0 then >= else <=) iDistance
    GETAICONDDATA(004);

    CCharacter* target = entity->GetCharTarget( );
//	CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
    if (target == NULL) return AI_FAILURE;
    target->UpdatePosition();
    entity->UpdatePosition();
    int distance = (int)GServer->distance( entity->Position->current, target->Position->current );
//	int distance = entity->basic.pos.distance(target->basic.pos);
    if (data->cMoreLess)
    {
        if (distance <= data->iDistance)
        {
            //Log(MSG_DEBUG, "Check Distance (2)> %i", data->iDistance);
            return AI_SUCCESS;
        }
    }
    else
    {
        if (distance >= data->iDistance)
        {
            //Log(MSG_DEBUG, "Check Distance (2)< %i", data->iDistance);
            return AI_SUCCESS;
        }
    }
    return AI_FAILURE;
}

//Check AB (1)
AICOND(005)
{
    /*
    byte cAbType;	//Pos: 0x00
    dword iDiff;	//Pos: 0x04
    byte cMoreLess;	//Pos: 0x08
    */
    GETAICONDDATA(005);

    //cAbType
    //0 = Level
    //1 = Attack
    //2 = Defense
    //3 = "Res"? Magic Resistance?
    //4 = HP
    //5 = Charm

    //Check if "ability" (cMoreLess == 0 then >= else <=) iDiff
//    Log(MSG_DEBUG, "Check AB (1)");
    int value = AI_GetAbility(entity, data->cAbType);
//	Log(MSG_DEBUG, "Check AB (1) type %i diff %i", data->cAbType, data->iDiff);
    if (value < 0) return AI_FAILURE;
    if (data->cMoreLess)
    {
        if (value > data->iDiff)
        {
//            Log(MSG_DEBUG, "Check AB (1) > %i", data->iDiff);
            return AI_SUCCESS;
        }
    }
    else
    {
        if (value < data->iDiff)
        {
//            Log(MSG_DEBUG, "Check AB (1) < %i", data->iDiff);
            return AI_SUCCESS;
        }
    }
    return AI_FAILURE;
}

//Check HP
AICOND(006)
{
    //dword wHP;	//Pos: 0x00
    //byte cMoreLess;	//Pos: 0x04

    //Check if hp% (cMoreLess == 0 then >= else <=) wHP
    GETAICONDDATA(006);
    dword value = (dword)(((float)entity->Stats->HP/ (float)entity->Stats->MaxHP) * 100.0f);
//	Log(MSG_DEBUG, "Check HP %i %i", data->cMoreLess, value);
    if (data->cMoreLess)
    {
        if (value <= data->wHP) return AI_SUCCESS;
    }
    else
    {
        if (value >= data->wHP) return AI_SUCCESS;
    }
    return AI_FAILURE;
}

//Random Chance
AICOND(007)
{
    //byte cPercent;	//Pos: 0x00
    //Random number 0->100 if random number < cPercent then return true
    GETAICONDDATA(007);
//	byte rand = rg.IRandom(0, 100) & 0xFF;
    //srand(time(NULL));
    byte brand = rand()%100;
//    Log(MSG_DEBUG, "brand =  %i chance = %i", brand, data->cPercent);
    if (brand < data->cPercent) return AI_SUCCESS;
    return AI_FAILURE;
}

//Check Near (2)
AICOND(008)
{
    //dword iDistance;	//Pos: 0x00
    //word nLevelDiff;	//Pos: 0x04
    //word nLevelDiff2;	//Pos: 0x06
    //byte btIsAllied;	//Pos: 0x08

    //find atleast 1 aiobj between nLevelDiff and nLevelDiff2 who is btIsAllied within iDistance

    GETAICONDDATA(008);

    dword eCount = 0;
    int searchDistance = data->iDistance;// * 10;
    CMap* map = GServer->MapList.Index[entity->Position->Map];


    if (data->btIsAllied)
    {
        dword entityCount = map->MonsterList.size();
        for (UINT j=0;j<map->MonsterList.size();j++)
        {
            CMonster* other = map->MonsterList.at(j);

            if (eCount >= entityCount) break;
            if (other == NULL) continue;
            eCount++;
            if (other == entity) continue;
            if (other->CharType == 0) continue;

            int levelDiff = abs(entity->Stats->Level - other->Stats->Level);
            if (levelDiff < data->nLevelDiff) continue;
            if (levelDiff > data->nLevelDiff2) continue;

            int iDistance = (int)GServer->distance( other->Position->current, entity->Position->current );
            if (iDistance > searchDistance) continue;

            entity->findChar = other;
            entity->nearChar = other;
            return AI_SUCCESS;
        }
    }
    else
    {
        dword entityCount = map->PlayerList.size();
        for (UINT j=0;j<map->PlayerList.size();j++)
        {
            CPlayer* other = map->PlayerList.at(j);

            if (eCount >= entityCount) break;
            if (other == NULL) continue;
            eCount++;
            if (other == entity) continue;
            if (other->CharType == 0) continue;

            int levelDiff = abs(entity->Stats->Level - other->Stats->Level);
            if (levelDiff < data->nLevelDiff) continue;
            if (levelDiff > data->nLevelDiff2) continue;

            int iDistance =(int) GServer->distance( other->Position->current, entity->Position->current );
            if (iDistance > searchDistance) continue;

            entity->findChar = other;
            entity->nearChar = other;
            return AI_SUCCESS;
        }
    }
    return AI_FAILURE;
}

//Unknown
AICOND(009)
{
    //Do i have a target?
//	CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
//	if(target == NULL){entity->_TargetID = 0; return AI_FAILURE;}
//	return AI_SUCCESS;
    CCharacter* target = entity->GetCharTarget( );
    if (target==NULL) return AI_FAILURE;
    return AI_SUCCESS;
}

//Check AB (2)
AICOND(010)
{
    //byte cAbType;	//Pos: 0x00
    //byte cMoreLess;	//Pos: 0x01

    //Is my ability >= or <= than my targets
    GETAICONDDATA(010);
//	CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
    CCharacter* target = entity->GetCharTarget( );
    if (target == NULL) return AI_FAILURE;
//Log(MSG_DEBUG, "Check AB (2) type %i ", data->cAbType);
    int myValue = AI_GetAbility(entity, data->cAbType);
    if (myValue < 0) return AI_FAILURE;

    int targetValue = AI_GetAbility(target, data->cAbType);
    if (targetValue < 0) return AI_FAILURE;

    if (data->cMoreLess)
    {
        if (myValue > targetValue) return AI_SUCCESS;
    }
    else
    {
        if (myValue < targetValue) return AI_SUCCESS;
    }
    return AI_FAILURE;
}

//Check AB (3)
AICOND(011)
{
    //byte cAbType;	//Pos: 0x00
    //dword iValue;	//Pos: 0x04
    //byte cMoreLess;	//Pos: 0x08
    //check ability of "m_pDestCHAR" <= or >= iValue
    GETAICONDDATA(011);
    dword tValue=data->iValue;
    if (tValue==0)tValue=AI_GetAbility(entity, data->cAbType);
    CCharacter* target = entity->GetCharTarget( );
    if (target == NULL) return AI_FAILURE;
    int myValue = AI_GetAbility(target, data->cAbType);
//	Log(MSG_DEBUG, "check AB (3) %i<>%i moreless %i type %i",myValue,tValue,data->cMoreLess,data->cAbType);
    if (data->cMoreLess)
    {
        if (myValue <= tValue) return AI_SUCCESS;
    }
    else
    {
        if (myValue >= tValue) return AI_SUCCESS;
    }
    return AI_FAILURE;
}

//Check Time (1)
AICOND(012)
{
    //byte cWhen;	//Pos: 0x00
    //Is it night or day? cWhen: ?0 = night 1 = day?
    GETAICONDDATA(012);
    CMap* map = GServer->MapList.Index[entity->Position->Map];
    if (data->cWhen)
    {
        if (map->CurrentTime>2)
            return AI_SUCCESS;
    }
    else
    {
        if (map->CurrentTime<3)
            return AI_SUCCESS;
    }
    return AI_FAILURE;
}

//Check Target (1)
AICOND(013)
{
    //byte btCheckTarget;	//Pos: 0x00 //0 = self 1 = target
    //byte btStatusType;	//Pos: 0x01 //
    //byte btHave;	//Pos: 0x02

    //Does my target btHave MagicSTATUS (btStatusType)
    //btCheckTarget does Get_MagicSTATUS but uses differ AND masks
    //btCheckTarget = 0 AND EAX,1EAAA84
    //btCheckTarget = 1 AND EAX,1E155570
    //btCheckTarget = 2 normal
    CCharacter* target = NULL;
    UINT buffs=0;
    GETAICONDDATA(013);
    if (data->btCheckTarget==1)
    {
        target=entity->GetCharTarget( );

    }
    else if (data->btCheckTarget==0)
    {
        target=entity;
    }
    else
    {
        Log(MSG_DEBUG, "Check Target (1) btCheckTarget %i ", data->btCheckTarget);
        return AI_FAILURE;
    }
    if (target==NULL)
    {
        Log(MSG_DEBUG, "Check Target (1)not found btCheckTarget %i ", data->btCheckTarget);
        return AI_FAILURE;
    }
//   Log(MSG_DEBUG, "Check Target (1) Target %i ",target->clientid);
    if (data->btStatusType)
    {
        buffs=GServer->BuildUpBuffs(target);
    }
    else
    {
        buffs=GServer->BuildDeBuffs(target);
    }
    if (data->btHave)
    {
        if (buffs)return AI_SUCCESS;
        return AI_FAILURE;
    }
    else
    {
        if (buffs)return AI_FAILURE;
        return AI_SUCCESS;
    }
    //CCharacter* target = entity->GetCharTarget( );
    Log(MSG_DEBUG, "Check Target (1) shouldn't be able to get this far");
    return AI_SUCCESS;
    //return AI_FAILURE;
}

//Check Variable (1) (ObjVar)
AICOND(014)
{
    GETAICONDDATA(014);
//	if(entity->_EntityType != ENTITY_NPC) return AI_FAILURE;
//	CNpc* thisNpc = reinterpret_cast<CNpc*>(entity);
//	thisNpc = thisNpc->SelectedNpc;
//	if(thisNpc == NULL) return AI_FAILURE;

//	short VarValue = thisNpc->ObjVar.GetVar(data->btVarIDX);
//	if(OperateValues<short>(data->btOp, &VarValue, data->iValue & 0xFFFF)) return AI_SUCCESS;
    return AI_FAILURE;
}

//Check Variable (2)
AICOND(015)
{
    //word nVarIDX;	//Pos: 0x00
    //byte btOp;	//Pos: 0x08
    //dword iValue;	//Pos: 0x04

    //Check WorldVAR
    return AI_FAILURE;
}

//Check Variable (3)
AICOND(016)
{
    //word nVarIDX;	//Pos: 0x00
    //dword iValue;	//Pos: 0x04
    //byte btOp;	//Pos: 0x08

    //Check EconomyVar
    return AI_FAILURE;
}

//Select NPC
AICOND(017)
{
    GETAICONDDATA(017);
//	if(entity->_EntityType != ENTITY_NPC) return AI_FAILURE;
//	CNpc* thisNpc = reinterpret_cast<CNpc*>(entity);
//	std::map<dword, CNpc*>::iterator triggerITR = server->NpcList.find(data->iNpcNo);
//	if(triggerITR == server->NpcList.end()){
//		thisNpc->SelectedNpc = NULL;
//		return AI_FAILURE;
//	}
//	thisNpc->SelectedNpc = triggerITR->second;

    //return AI_SUCCESS;
    return AI_FAILURE;
}

//Check Distance (3)
AICOND(018)
{
    //dword iDistance;	//Pos: 0x00
    //byte btOp;	//Pos: 0x04

    //Distance to "CALLER" ??Possibly the one who summoned the monster??
    GETAICONDDATA(018);
    if (!entity->IsMonster( )) return AI_FAILURE;
//	Log(MSG_DEBUG, "AICOND(018 1)");
    CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
    CMap* map = GServer->MapList.Index[thisMonster->Position->Map];
    CCharacter* caller = map->GetCharInMap( thisMonster->owner );
    if (caller == NULL) return AI_FAILURE;
//	Log(MSG_DEBUG, "AICOND(018 2)");
//	if(entity->_EntityType != ENTITY_MONSTER) return AI_FAILURE;
//	CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
//	CWorldEntity* caller = thisMonster->thisZone->GetEntity(thisMonster->_CallerID);
//	if(caller == NULL) return AI_FAILURE;
    caller->UpdatePosition();
    thisMonster->UpdatePosition();
    int distance = (int)GServer->distance( caller->Position->current, thisMonster->Position->current );
//	int distance = thisMonster->basic.pos.distance(caller->basic.pos);
//Log(MSG_DEBUG, "AICOND(018 3)");
    if (OperateValues<int>(data->btOp, &distance, data->iDistance)) return AI_SUCCESS;
//	Log(MSG_DEBUG, "AICOND(018 4)");
    return AI_FAILURE;
}

//Check Time (2)
AICOND(019)
{
    //dword ulTime;	//Pos: 0x00
    //dword ulEndTime;	//Pos: 0x04

    //Get_ZoneTIME between ulTime and ulEndTime
    return AI_FAILURE;
}

//Check AB (4)
AICOND(020)
{
    //byte btAbType;	//Pos: 0x00
    //byte btOp;	//Pos: 0x08
    //dword iValue;	//Pos: 0x04

    //cAbType
    //0 = Level
    //1 = Attack
    //2 = Defense
    //3 = "Res"? Magic Resistance?
    //4 = HP
    //5 = Charm
    GETAICONDDATA(020);
//Log(MSG_DEBUG, "Check AB (4) type %i iValue %i", data->btAbType, data->iValue);
    int value = AI_GetAbility(entity, data->btAbType);
    if (value < 0) return AI_FAILURE;
    if (OperateValues<int>(data->btOp, &value, data->iValue)) return AI_SUCCESS;
//	Log(MSG_INFO,"check ab (4) successfull");
    return AI_FAILURE;
}

//Unknown
AICOND(021)
{
    //Do i have a "CALLER" ??Possibly the one who summoned the monster??
    GETAICONDDATA(021);
//	Log(MSG_DEBUG, "AICOND(021) Do i have a 'CALLER'");

    if (!entity->IsMonster())return AI_FAILURE;
    CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
//Log(MSG_DEBUG, "AICOND(021) Checking now");
//	if(entity->_EntityType != ENTITY_MONSTER) return AI_FAILURE;
//	CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
//	CWorldEntity* caller = thisMonster->thisZone->GetEntity(thisMonster->_CallerID);
    if (thisMonster->owner == 0) return AI_SUCCESS;
//	Log(MSG_DEBUG, "AICOND(021) Yes I Do");
    return AI_FAILURE;
}

//Unknown
AICOND(022)
{
    //Does my "CALLER" have a target?
//	Log(MSG_DEBUG, "AICOND(022) Does my `CALLER` have a target?");
    GETAICONDDATA(022);

    if (!entity->IsMonster( )) return AI_FAILURE;
//	Log(MSG_DEBUG, "AICOND(022 1)");
    CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
    CMap* map = GServer->MapList.Index[thisMonster->Position->Map];
    CCharacter* caller = map->GetCharInMap( thisMonster->owner );
    if (caller == NULL) return AI_FAILURE;
//	Log(MSG_DEBUG, "AICOND(022 2)");
    CCharacter* target = map->GetCharInMap(caller->Battle->target);
    if (target != NULL && target != entity) return AI_SUCCESS;
//	Log(MSG_DEBUG, "AICOND(022 3)");
    return AI_FAILURE;
}

//Check Time (3) (Game map time?)
AICOND(023)
{
    //dword ulTime;	//Pos: 0x00
    //dword ulEndTime;	//Pos: 0x04

    //Get_WorldTIME between ulTime and ulEndTime
    return AI_FAILURE;
}

//Check Date Time (4)
AICOND(024)
{
    GETAICONDDATA(024);

//	SYSTEMTIME sTIME;
//	GetLocalTime(&sTIME);

//	if(data->btDate != 0){
//		if(sTIME.wDay != data->btDate)
//			return AI_FAILURE;
//	}
//	word wMin = ((sTIME.wHour * 60) + sTIME.wMinute);
//	word wFrom = (data->btHour1 * 60) + data->btMin1;
//	word wTo = (data->btHour2 * 60) + data->btMin2;
//	if(wMin >= wFrom && wMin <= wTo)
//		return AI_SUCCESS;
    return AI_FAILURE;
}

//Check Weekday Time (5)
AICOND(025)
{
    GETAICONDDATA(025);

//	SYSTEMTIME sTIME;
//	GetLocalTime(&sTIME);

//	if(data->btWeekDay != sTIME.wDayOfWeek) return AI_FAILURE;
//	word wMin = ((sTIME.wHour * 60) + sTIME.wMinute);
//	word wFrom = (data->btHour1 * 60) + data->btMin1;
//	word wTo = (data->btHour2 * 60) + data->btMin2;
//	if(wMin > wFrom && wMin < wTo) return AI_SUCCESS;
    return AI_FAILURE;
}

//Check Server/Channel
AICOND(026)
{
    GETAICONDDATA(026);
    //channel >= min && channel <= max
    //return AI_SUCCESS;
    return AI_FAILURE;
}

//Check Near Character
AICOND(027)
{
    //dword iDistance;	//Pos: 0x00
    //byte btIsAllied;	//Pos: 0x04
    //word nLevelDiff;	//Pos: 0x06
    //word nLevelDiff2;	//Pos: 0x08
    //word wChrNum;	//Pos: 0x0a
    //byte btOp;	//Pos: 0x0c

    //count how many aiobj between nLevelDiff and nLevelDiff2 who is btIsAllied within iDistance
    //must be (btOp) wChrNum
    GETAICONDDATA(027);

    short chrCount = 0;
    dword eCount = 0;
    int nearestDistance = 9999999;
    int searchDistance = data->iDistance;// * 10;

    CMap* map = GServer->MapList.Index[entity->Position->Map];
    dword entityCount = map->MonsterList.size();

    if (data->btIsAllied)
    {
        for (UINT j=0;j<map->MonsterList.size();j++)
        {
            CMonster* other = map->MonsterList.at(j);
            if (eCount >= entityCount) break;
            if (other == NULL) continue;
            eCount++;
            if (other == entity) continue;
            int levelDiff = (entity->Stats->Level - other->Stats->Level);
            if (levelDiff < data->nLevelDiff) continue;
            if (levelDiff > data->nLevelDiff2) continue;
            int iDistance = (int)GServer->distance( other->Position->current, entity->Position->current );
            if (iDistance > searchDistance) continue;
            chrCount++;
            if (iDistance < nearestDistance)
            {
                nearestDistance = iDistance;
                entity->nearChar = other;
            }
        }
        entity->findChar = entity->nearChar;
        if (OperateValues<short>(data->btOp, &chrCount, data->wChrNum)) return AI_SUCCESS;
    }
    else
    {
        for (UINT j=0;j<map->PlayerList.size();j++)
        {
            CPlayer* other = map->PlayerList.at(j);
            if (eCount >= entityCount) break;
            if (other == NULL) continue;
            eCount++;
            if (other == entity) continue;
            int levelDiff = (entity->Stats->Level - other->Stats->Level);
            if (levelDiff < data->nLevelDiff) continue;
            if (levelDiff > data->nLevelDiff2) continue;
            int iDistance = (int)GServer->distance( other->Position->current, entity->Position->current );
            if (iDistance > searchDistance) continue;
            chrCount++;
            if (iDistance < nearestDistance)
            {
                nearestDistance = iDistance;
                entity->nearChar = other;
            }
        }
        entity->findChar = entity->nearChar;
        if (OperateValues<short>(data->btOp, &chrCount, data->wChrNum)) return AI_SUCCESS;
    }
    return AI_FAILURE;
}

//Check Variable (4)
AICOND(028)
{
    //word nVarIDX;	//Pos: 0x00
    //byte btOp;	//Pos: 0x08
    //dword iValue;	//Pos: 0x04

    //Get_AiVAR
    return AI_FAILURE;
}

//Check Target (2)
AICOND(029)
{
    //byte btTargetType;	//Pos: 0x00
    //UNKNOWN :@
    GETAICONDDATA(029);
    Log(MSG_DEBUG,"aicon 029 check target type %i",data->btTargetType);
    return AI_FAILURE;
}

//Unknown
AICOND(030)
{
    return AI_FAILURE;
}
