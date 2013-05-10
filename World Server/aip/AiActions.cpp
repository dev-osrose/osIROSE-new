// Props to ExJam for this code :D
#include "../WorldServer.h"

// extern CRandomMersenne rg;

//Unknown
AIACT(000)
{
    //SetCMD_Stop (Stop all actions i guess?)
//	entity->StopAll();
    return AI_SUCCESS;
}

//Do Action
AIACT(001)
{
    //byte cAction;	//Pos: 0x00
    //CObjMOB::Set_EMOTION (pak 781)
    GETAIACTDATA(001);
    BEGINPACKET( pak, 0x781 );
    ADDWORD    ( pak, data->cAction );
    ADDWORD    ( pak, 0);
    ADDWORD    ( pak, entity->clientid );
    GServer->SendToVisible(&pak, entity,NULL );
    return AI_SUCCESS;
}

//Say LTB String
AIACT(002)
{
    //Client side say text
    //Log(MSG_DEBUG, "Say LTB String");
    return AI_SUCCESS;
}

//Move (1)
AIACT(003)
{
    //dword iDistance;	//Pos: 0x00
    //byte cSpeed;	//Pos: 0x04
    //move randomly within iDistance (individual x and y so its within a square)
    GETAIACTDATA(003);
    CMonster* monster = reinterpret_cast<CMonster*>(entity);
    monster->UpdatePosition( );
    int iDist = data->iDistance;// * 2;//Get it to our coord system!

//	int randDis = rg.IRandom(0, iDist << 1);
//    srand(time(NULL));
    int randDis = rand()%(iDist << 1);
    float nX = (entity->Position->current.x + randDis) - iDist;
    entity->Position->destiny.x=nX;

//	randDis = rg.IRandom(0, iDist << 1);
    //srand(time(NULL));
    randDis = rand()%(iDist << 1);
    float nY = (entity->Position->current.y + randDis) - iDist;
    entity->Position->destiny.y=nY;
    monster->thisnpc->stance = data->cSpeed;
    monster->SetStats();
    entity->Position->lastMoveTime = clock();
    //ClearBattle( entity->Battle );// this also seems to clear the attackers battle??
    entity->Battle->atktype=0;
    BEGINPACKET( pak, 0x797 );
    ADDWORD    ( pak, entity->clientid );
    ADDWORD    ( pak, 0x0000 );
    ADDWORD    ( pak, entity->Stats->Move_Speed ); //speed
    ADDFLOAT   ( pak, entity->Position->destiny.x*100 );
    ADDFLOAT   ( pak, entity->Position->destiny.y*100 );
    ADDWORD    ( pak, 0xcdcd );
    ADDBYTE    ( pak, monster->thisnpc->stance );
    GServer->SendToVisible(&pak, entity);
//	Log(MSG_DEBUG, "move1 stance %i",data->cSpeed);
//Log(MSG_DEBUG, "move(1)");
    return AI_SUCCESS;
}

//Move (2)
AIACT(004)
{
    //dword iDistance;	//Pos: 0x00
    //byte cSpeed;	//Pos: 0x04
    //move randomly within iDistance of spawn position (square)	Position->source
    GETAIACTDATA(004);
    CMonster* monster = reinterpret_cast<CMonster*>(entity);
    monster->UpdatePosition( );

    int iDist = data->iDistance;//Get it to our coord system!

//	int randDis = rg.IRandom(0, iDist << 1);
    int randDis = rand()%(iDist << 1);
    float nX = (entity->Position->source.x + randDis) - iDist;
    entity->Position->destiny.x=nX;

//	randDis = rg.IRandom(0, iDist << 1);
    randDis = rand()%(iDist << 1);
    float nY = (entity->Position->source.y + randDis) - iDist;
    entity->Position->destiny.y=nY;


    monster->thisnpc->stance = data->cSpeed;
    monster->SetStats();
    entity->Position->lastMoveTime = clock();
//    ClearBattle( entity->Battle );// this also seems to clear the attackers battle??
    entity->Battle->atktype=0;
    BEGINPACKET( pak, 0x797 );
    ADDWORD    ( pak, entity->clientid );
    ADDWORD    ( pak, 0x0000 );//???
    ADDWORD    ( pak, entity->Stats->Move_Speed ); //speed
    ADDFLOAT   ( pak, entity->Position->destiny.x*100 );
    ADDFLOAT   ( pak, entity->Position->destiny.y*100 );
    ADDWORD    ( pak, 0xcdcd );
    ADDBYTE    ( pak, monster->thisnpc->stance );
    GServer->SendToVisible(&pak, entity);
//	Log(MSG_DEBUG, "move2 stance %i",data->cSpeed);

    return AI_SUCCESS;
}

//Move (3)
AIACT(005)
{
    //byte cSpeed;	//Pos: 0x00
    //move randomly within 200 of "m_pFindCHAR" (square)
    GETAIACTDATA(005);
    if (entity->findChar == NULL) return AI_FAILURE;
    CMonster* monster = reinterpret_cast<CMonster*>(entity);
    monster->UpdatePosition( );
    entity->findChar->UpdatePosition();

    int iDist = 2;//Get it to our coord system!


    int randDis = rand()%(iDist << 1);
    float nX = (entity->findChar->Position->current.x + randDis) - iDist;
    entity->Position->destiny.x=nX;


    randDis = rand()%(iDist << 1);
    float nY = (entity->findChar->Position->current.y + randDis) - iDist;
    entity->Position->destiny.y=nY;
    monster->thisnpc->stance = data->cSpeed;
    monster->SetStats();
//	entity->MoveTo(nX, nY);
//   entity->Position->lastMoveTime = clock();
    //ClearBattle( entity->Battle );// this also seems to clear the attackers battle??
    entity->Battle->atktype=0;
    BEGINPACKET( pak, 0x797 );
    ADDWORD    ( pak, entity->clientid );
    ADDWORD    ( pak, 0x0000 );//???
    ADDWORD    ( pak, entity->Stats->Move_Speed ); //speed
    ADDFLOAT   ( pak, entity->Position->destiny.x*100 );
    ADDFLOAT   ( pak, entity->Position->destiny.y*100 );
    ADDWORD    ( pak, 0xcdcd );
    ADDBYTE    ( pak, monster->thisnpc->stance );
    GServer->SendToVisible(&pak, entity);
//	Log(MSG_DEBUG, "move3 stance %i",data->cSpeed);
//	Log(MSG_DEBUG, "move (3)");
//    RESETPACKET(pak, 0x783);
//    ADDWORD(pak,entity->findChar->clientid);//entity->clientid);
//    ADDSTRING(pak,"hello there");
//    ADDBYTE(pak,0);
//    GServer->SendToVisible(&pak, entity);
//		server->SendPacketToZone(entity, &pakout);

    return AI_SUCCESS;
}

//Move (?)
AIACT(006)
{
    //dword iDistance;	//Pos: 0x00
    //byte cAbType;	//Pos: 0x04
    //byte cMoreLess;	//Pos: 0x05
    //Run and Attack aiobj within iDistance that has the lowest or greatest cAbType
    GETAIACTDATA(006);

//	dword eCount = 0;
//	int highestAB = -9999999;
//	CWorldEntity* highestEntity = NULL;
//	int lowestAB = 9999999;
//	CWorldEntity* lowestEntity = NULL;

//	int searchDistance = data->iDistance * 100;

//	CWorldEntity** entityList = entity->thisZone->GetEntityList();
//	dword entityCount = entity->thisZone->GetEntityCount();
//	for(dword i = 1; i < MAX_ZONE_CLIENTID; i++){
//		CWorldEntity* other = entityList[i];
//		if(eCount >= entityCount) break;
//		if(other == NULL) continue;
//		eCount++;
//		if(other == entity) continue;
//		if(other->_EntityType == ENTITY_NO_TYPE) continue;
//		if(other->_EntityType == ENTITY_DROP) continue;

//		int dX = (int)abs((int)other->curBlock.x - (int)entity->curBlock.x);
//		if(dX > 1) continue;
//		int dY = (int)abs((int)other->curBlock.y - (int)entity->curBlock.y);
//		if(dY > 1) continue;

//		int iDistance = other->basic.pos.distance(entity->basic.pos);
//		if(iDistance > searchDistance) continue;

//		int value = AI_GetAbility(other, data->cAbType);

//		if(value < lowestAB){
//			lowestAB = value;
//			lowestEntity = other;
//		}
//		if(value > highestAB){
//			highestAB = value;
//			highestEntity = other;
//		}
//	}
//	if(data->cMoreLess){
//		if(lowestEntity){
    //SetCmdRUN_nATTACK
//			entity->Attack(lowestEntity);
//		}
//	}else{
//		if(highestEntity){
    //SetCmdRUN_nATTACK
//			entity->Attack(highestEntity);
//		}
//	}
    Log(MSG_DEBUG, "move (?)");
    return AI_SUCCESS;
}

//Unknown
AIACT(007)
{
    //CObjCHAR::Special_ATTACK
    Log(MSG_DEBUG, "AIACT(007)");
    return AI_SUCCESS;
}

//Move (4)
AIACT(008)
{
    //dword iDistance;	//Pos: 0x00
    //byte cSpeed;	//Pos: 0x04
    //Some weird shit going on here
    //move within some sort of distance of target (i think possibly % iDistance of distance between target)(run away?)
    //Log(MSG_DEBUG, "AIACT(008)");
    return AI_SUCCESS;
}

//Spawn Monster (1)
AIACT(009)
{
    //word wMonster;	//Pos: 0x00
    //Transform into wMonster
    GETAIACTDATA(010);
    CMap* map= GServer->MapList.Index[entity->Position->Map];
    CMonster* monster = reinterpret_cast<CMonster*>(entity);
    map->ConverToMonster(monster,data->wMonster,1);
    //Log(MSG_DEBUG, "AIACT(009)%i to %i",entity->CharType,data->wMonster);
    return AI_SUCCESS;
}

//Spawn Monster (2)
AIACT(010)
{
    //word wMonster;	//Pos: 0x00
    //Spawn wMonster at current X,Y
    GETAIACTDATA(010);
    CMap* map = GServer->MapList.Index[entity->Position->Map];
    fPoint position;
    //CMonster* newmonster;
    position = GServer->RandInCircle( entity->Position->current, 1 );
    map->AddMonster( data->wMonster, position, 0, NULL, NULL, 0, true );
//	entity->thisZone->SpawnMonster(data->wMonster, entity->basic.pos, entity->basic.map);
//Log(MSG_DEBUG, "AIACT(010)spawn %i",data->wMonster);
    return AI_SUCCESS;
}

//Spawn Monster (3)
AIACT(011)
{
    //dword iDistance;	//Pos: 0x00
    //dword iNumOfMonster;	//Pos: 0x04
    //force iNumOfMonster of same team within iDistance to attack my target
//	CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
//	if(target == NULL) return AI_FAILURE;
    //Log(MSG_DEBUG, "AIACT(011)");
    CCharacter* target = entity->GetCharTarget( );
    if (target == NULL) return AI_FAILURE;
    GETAIACTDATA(011);

    int chrCount = 0;
    dword eCount = 0;
    int searchDistance = data->iDistance;

    CMap* map = GServer->MapList.Index[entity->Position->Map];
    dword entityCount = map->MonsterList.size();
    for (UINT j=0;j<map->MonsterList.size();j++)
    {
        CMonster* other = map->MonsterList.at(j);
        if (eCount >= entityCount) break;
        if (other == NULL) continue;
        eCount++;
        if (other == entity) continue;
        if (other->CharType != 0) continue;

//	CWorldEntity** entityList = entity->thisZone->GetEntityList();
//	dword entityCount = entity->thisZone->GetEntityCount();
//	for(dword i = 1; i < MAX_ZONE_CLIENTID; i++){
//		CWorldEntity* other = entityList[i];
//		if(eCount >= entityCount) break;
//		if(other == NULL) continue;
//		eCount++;
//		if(other == entity) continue;
//		if(other->_EntityType == ENTITY_MONSTER) continue;
//		if(other->team != entity->team) continue;
//		int dX = (int)abs((int)other->curBlock.x - (int)entity->curBlock.x);
//		if(dX > 1) continue;
//		int dY = (int)abs((int)other->curBlock.y - (int)entity->curBlock.y);
//		if(dY > 1) continue;
        int iDistance =(int) GServer->distance( other->Position->current, entity->Position->current );
//		int iDistance = other->basic.pos.distance(entity->basic.pos);
        if (iDistance > searchDistance) continue;
        chrCount++;
        other->Battle->target=entity->Battle->target;
        other->StartAction( (CCharacter*) target, NORMAL_ATTACK, 0 );

//		other->Attack(target);

        if (chrCount >= data->iNumOfMonster) return AI_SUCCESS;
    }

    return AI_SUCCESS;
}

//Unknown
AIACT(012)
{
    Log(MSG_DEBUG, "AIACT(012)");
    //Run and attack "m_pNearCHAR" Nearest Character
//	entity->Attack(entity->nearChar);
//    Log(MSG_DEBUG, "AIACT(012)");
    if (entity->nearChar==NULL)return AI_FAILURE;
    //entity->Battle->target=entity->nearChar->clientid;
    CMonster* monster = reinterpret_cast<CMonster*>(entity);
    monster->thisnpc->stance=1;
    monster->SetStats();
    monster->StartAction( (CCharacter*) entity->nearChar, NORMAL_ATTACK, 0 );
//    Log(MSG_DEBUG, "AIACT(012)%i",entity->nearChar->clientid);
    return AI_SUCCESS;
}

//Unknown
AIACT(013) //Log(MSG_DEBUG, "AIACT(013)");
{
    //Run and attack "m_pFindCHAR" Character found from aiobj loop things
//	entity->Attack(entity->findChar);
    if (entity->findChar==NULL)return AI_FAILURE;
    //entity->Battle->target=entity->findChar->clientid;
    CMonster* monster = reinterpret_cast<CMonster*>(entity);
    monster->thisnpc->stance=1;
    monster->SetStats();
    monster->Battle->target=entity->findChar->clientid;
    monster->StartAction( (CCharacter*) entity->findChar, NORMAL_ATTACK, 0 );
//    Log(MSG_DEBUG, "AIACT(013)%i",entity->findChar->clientid);
    return AI_SUCCESS;
}

//Unknown
AIACT(014)
{
    //dword iDistance;	//Pos: 0x00
    //find aiobj within iDistance and force them to attack my target
//	CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
//	if(target == NULL) return AI_FAILURE;
    GETAIACTDATA(014);

//	dword eCount = 0;
//	int searchDistance = data->iDistance * 100;

//	CWorldEntity** entityList = entity->thisZone->GetEntityList();
//	dword entityCount = entity->thisZone->GetEntityCount();
//	for(dword i = 1; i < MAX_ZONE_CLIENTID; i++){
//		CWorldEntity* other = entityList[i];
//		if(eCount >= entityCount) break;
//		if(other == NULL) continue;
//		eCount++;
//		if(other == entity) continue;
//		if(other->_EntityType == ENTITY_NO_TYPE) continue;
//		if(other->_EntityType == ENTITY_DROP) continue;
//		if(other->team != entity->team) continue;

//		int dX = (int)abs((int)other->curBlock.x - (int)entity->curBlock.x);
//		if(dX > 1) continue;
//		int dY = (int)abs((int)other->curBlock.y - (int)entity->curBlock.y);
//		if(dY > 1) continue;

//		int iDistance = other->basic.pos.distance(entity->basic.pos);
//		if(iDistance > searchDistance) continue;

//		other->Attack(target);
//		return AI_SUCCESS;
//	}
    Log(MSG_DEBUG, "AIACT(014)");
    return AI_SUCCESS;
}

//Retaliate ?
AIACT(015)
{
    //Run and attack "m_pDestCHAR" Blah?
//	Log(MSG_DEBUG, "AIACT(015)");
    return AI_FAILURE;
}

//Unknown
AIACT(016)
{
    //dword iDistance;
    //Run Away!
    //Log(MSG_DEBUG, "AIACT(016)");
    GETAIACTDATA(016);
    CMonster* monster = reinterpret_cast<CMonster*>(entity);
    monster->UpdatePosition( );

    float radius = data->iDistance;
    float angle = rand()%(360);
//	float angle = rg.IRandom(0,360);
//	entity->UpdatePosition();
//	entity->MoveTo(entity->basic.pos.x + (radius * cos(angle)), entity->basic.pos.y + (radius * sin(angle)));
    float nX = (entity->Position->source.x + (radius * cos(angle)));
    entity->Position->destiny.x=nX;
    float nY = (entity->Position->source.y + (radius * cos(angle)));
    entity->Position->destiny.y=nY;

    monster->thisnpc->stance = 1;
    monster->SetStats();

    entity->Position->lastMoveTime = clock();
    //ClearBattle( entity->Battle );// this also seems to clear the attackers battle??
    entity->Battle->atktype=0;
    BEGINPACKET( pak, 0x797 );
    ADDWORD    ( pak, entity->clientid );
    ADDWORD    ( pak, 0x0000 );//???
    ADDWORD    ( pak, entity->Stats->Move_Speed ); //speed
    ADDFLOAT   ( pak, entity->Position->destiny.x*100 );
    ADDFLOAT   ( pak, entity->Position->destiny.y*100 );
    ADDWORD    ( pak, 0xcdcd );
    ADDBYTE    ( pak, 0x01 );
    GServer->SendToVisible(&pak, entity);


    return AI_SUCCESS;
}

//Drop Item
AIACT(017)
{
    //word item0;	//Pos: 0x00
    //word item1;	//Pos: 0x02
    //word item2;	//Pos: 0x04
    //word item3;	//Pos: 0x06
    //word item4;	//Pos: 0x08
    //dword iToOwner;	//Pos: 0x0c
    //Random drop one of item 1-5
    GETAIACTDATA(017);
    //srand(time(NULL));
    int itemRand = rand()%(5);
    //int itemRand = rg.IRandom(0,360);
    int nItem = data->items[itemRand];
    if (nItem==0)return AI_FAILURE;

    //CItem dropItem;
    CDrop* newdrop = new (nothrow) CDrop;
    newdrop->clientid = GServer->GetNewClientID( );
    newdrop->posMap = entity->Position->Map;
    newdrop->pos = GServer->RandInCircle( entity->Position->current, 3 );
    newdrop->droptime = time(NULL);
    newdrop->owner = 0;//entity->MonsterDrop->firsthit;
    newdrop->thisparty = 0;//entity->thisparty;
    ClearItem(newdrop->item);
    newdrop->amount = 1;
    newdrop->type = 2;
    newdrop->item.itemnum = nItem % 1000;
    newdrop->item.itemtype = nItem / 1000;
    if (newdrop->item.IsStackable())
    {
        newdrop->item.count = 1;
    }
    else
    {
        newdrop->item.stats = 0;
        newdrop->item.durability = 40;
        newdrop->item.lifespan = 1000;
        newdrop->item.socketed = 0;
        newdrop->item.appraised = 1;
        newdrop->item.refine = 0;
    }
//	Log(MSG_DEBUG, "item type=  %i item num = %i", newdrop->item.itemtype, newdrop->item.itemnum);
    newdrop->item.count = 1;


    CMap* map = GServer->MapList.Index[newdrop->posMap];
    map->AddDrop( newdrop );
//	entity->DropEntityItem(&dropItem);

    return AI_SUCCESS;
}

//Spawn Monster (4)
AIACT(018)
{
    //word cMonster;	//Pos: 0x00
    //word wHowMany;	//Pos: 0x02
    //dword iDistance;	//Pos: 0x04
    //make wHowMany monsters of type cMonster within iDistance attack my target
//	CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
//	if(target == NULL) return AI_FAILURE;
    GETAIACTDATA(018);

//	int chrCount = 0;
//	dword eCount = 0;
//	int nearestDistance = 9999999;
//	int searchDistance = data->iDistance * 100;

//	CWorldEntity** entityList = entity->thisZone->GetEntityList();
//	dword entityCount = entity->thisZone->GetEntityCount();
//	for(dword i = 1; i < MAX_ZONE_CLIENTID; i++){
//		CWorldEntity* other = entityList[i];
//		if(eCount >= entityCount) break;
//		if(other == NULL) continue;
//		eCount++;
//		if(other == entity) continue;
//		if(other->_EntityType != ENTITY_MONSTER) continue;
//		if(other->team != entity->team) continue;
//		if(reinterpret_cast<CMonster*>(other)->MonID != data->cMonster) continue;

//		int dX = (int)abs((int)other->curBlock.x - (int)entity->curBlock.x);
//		if(dX > 1) continue;
//		int dY = (int)abs((int)other->curBlock.y - (int)entity->curBlock.y);
//		if(dY > 1) continue;

//		int iDistance = other->basic.pos.distance(entity->basic.pos);
//		if(iDistance > searchDistance) continue;
//		chrCount++;

    //Run and Attack target
//		other->Attack(target);

//		if(chrCount >= data->wHowMany) return AI_SUCCESS;
//	}
    Log(MSG_DEBUG, "AIACT(018)");
    return AI_SUCCESS;
}

//Unknown
AIACT(019)
{
    //Run and attack "m_pNearCHAR" //Identical to 012
    return F_AI_ACT_012(server, entity, raw);
}

//Spawn Monster (5)
AIACT(020)
{
    //word cMonster;	//Pos: 0x00
    //byte btPos;	//Pos: 0x02
    //dword iDistance;	//Pos: 0x04

    //Spawn cMonster within iDistance of btPos

    //btPos 0 = my pos
    //btPos 1 = "m_pDestCHAR" pos
    //btPos 2 = targets pos
    GETAIACTDATA(020);
    CMap* map = GServer->MapList.Index[entity->Position->Map];
    fPoint position;

    if (data->btPos == 0)
    {
        position = GServer->RandInCircle( entity->Position->current, data->iDistance );
        map->AddMonster( data->cMonster, position, 0, NULL, NULL, 0, true );
//		entity->thisZone->SpawnMonster(data->cMonster, entity->basic.pos, entity->basic.map, 1, data->iDistance * 100);
    }
    else if (data->btPos == 1)
    {
        position = GServer->RandInCircle( entity->Position->current, data->iDistance );
        map->AddMonster( data->cMonster, position, 0, NULL, NULL, 0, true );
        //dest char
    }
    else if (data->btPos == 2)
    {
        CCharacter* target = entity->GetCharTarget( );
        if (target == NULL) return AI_FAILURE;
        position = GServer->RandInCircle( target->Position->current, data->iDistance );
        map->AddMonster( data->cMonster, position, 0, NULL, NULL, 0, true );
//		CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
//		if(target == NULL) return AI_FAILURE;
//		entity->thisZone->SpawnMonster(data->cMonster, target->basic.pos, target->basic.map, 1, data->iDistance * 100);
    }


//Log(MSG_DEBUG, "AIACT(020)option %i distance %i monstertype %i",data->btPos,data->iDistance,data->cMonster);
    return AI_SUCCESS;
}

//Unknown
AIACT(021)
{
    //Does absolutely nothing
    return AI_SUCCESS;
}

//Unknown
AIACT(022)
{
    //Does absolutely nothing
    return AI_SUCCESS;
}

//Unknown //ghostseeds and ghosts and player summons
AIACT(023)
{
    //Commit suicide
    entity->Stats->HP=0;
    BEGINPACKET( pak, 0x799 );
    ADDWORD    ( pak, entity->clientid );
    ADDWORD    ( pak, entity->clientid );
    ADDDWORD   ( pak, 0x8005 );
    GServer->SendToVisible( &pak, entity );
    return AI_SUCCESS;
}

//Do Skill
AIACT(024)
{
    //byte btTarget;	//Pos: 0x00
    //word nSkill;	//Pos: 0x02
    //word nMotion;	//Pos: 0x04 //8 = SKILL_ATTACK 6 = magic

    //Use nSkill & nMotion?
    //btTarget 0 = m_pFindCHAR
    //btTarget 1 = target
    //btTarget 2 = self skill
    GETAIACTDATA(024);
    Log(MSG_DEBUG, "Do Skill btTarget %i nSkill %i nMotion %i",data->btTarget,data->nSkill,data->nMotion);
//	Log(MSG_DEBUG, "entity %i",entity->clientid);
    CSkills* thisskill = GServer->GetSkillByID( data->nSkill );
    Log(MSG_DEBUG, "skill target type %i", thisskill->target);
    CCharacter* monster =entity;
    if (data->btTarget==0)
    {
        if (thisskill->target==tHostileCharacter)
        {
            CCharacter* target = entity->findChar;
            if (target==NULL)
            {
                Log(MSG_DEBUG, "target0 == 0");
                return AI_FAILURE;
            }
            if (data->nMotion==8)
            {
                monster->StartAction( (CCharacter*) target, SKILL_ATTACK, data->nSkill );
            }
            else
            {
                monster->StartAction( (CCharacter*) target, SKILL_BUFF, data->nSkill );
            }
        }
    }
    else if (data->btTarget==1)
    {
        CCharacter* target = entity->GetCharTarget( );
        if (target==NULL)
        {
            Log(MSG_DEBUG, "target1 == 0");
            return AI_FAILURE;
        }
        if (data->nMotion==8)
        {
            monster->StartAction( (CCharacter*) target, SKILL_ATTACK, data->nSkill );
        }
        else
        {
            monster->StartAction( (CCharacter*) target, SKILL_BUFF, data->nSkill );
        }
    }
    else if (data->btTarget==2)
    {
        if (thisskill->target==tYourself)
        {
            monster->StartAction( entity, BUFF_SELF, data->nSkill );
        }
        else if (thisskill->target==tAlly)
        {
            monster->StartAction( NULL, BUFF_AOE, data->nSkill );
        }
        else if (thisskill->target==tHostileCharacter)
        {
            CCharacter* target = entity->GetCharTarget( );
            if (target==NULL)return AI_FAILURE;
            if (data->nMotion==8)
            {
                monster->StartAction( (CCharacter*) target, SKILL_ATTACK, data->nSkill );
            }
            else
            {
                monster->StartAction( (CCharacter*) target, SKILL_BUFF, data->nSkill );
            }
        }
    }
    return AI_SUCCESS;
}

//Set Variable (1)
AIACT(025)
{
//	if(entity->_EntityType != ENTITY_NPC) return AI_FAILURE;
    GETAIACTDATA(025);

//	CNpc* thisNpc = reinterpret_cast<CNpc*>(entity);
//	thisNpc = thisNpc->SelectedNpc;
//	if(thisNpc == NULL)
//		return AI_FAILURE;

//	short VarValue = thisNpc->ObjVar.GetVar(data->btVarIDX);
//	OperateValues(data->btOp, &VarValue, (short)data->iValue);
//	thisNpc->ObjVar.SetVar(data->btVarIDX, VarValue);

    return AI_SUCCESS;
}

//Set Variable (2)
AIACT(026)
{
    //Set WorldVAR
    //word nVarNo;	//Pos: 0x00
    //byte btOp;	//Pos: 0x08
    //dword iValue;	//Pos: 0x04
    return AI_SUCCESS;
}

//Set Variable (3)
AIACT(027)
{
    //Set EconomyVAR
    //word nVarNo;	//Pos: 0x00
    //byte btOp;	//Pos: 0x08
    //dword iValue;	//Pos: 0x04
    return AI_SUCCESS;
}

//Shout/Ann LTB String
AIACT(028)
{
    GETAIACTDATA(028);

//	dword npcId = 0;
//	if(entity->_EntityType == ENTITY_NPC){
//		npcId = reinterpret_cast<CNpc*>(entity)->NpcID;
//	}else if(entity->_EntityType == ENTITY_MONSTER){
//		npcId = reinterpret_cast<CMonster*>(entity)->MonID;
//	}else{
//		return AI_FAILURE;
//	}
//	std::map<dword, char*>::iterator triggerITR = server->NpcNames.find(npcId);
//	if(triggerITR == server->NpcNames.end()) return AI_FAILURE;

//	char* npcName = triggerITR->second;
//	char* sayStr = server->lngAi->Data(data->iStrID, 1);
//	if(sayStr == NULL) return AI_FAILURE;
//	if(data->btMsgType == 0){
    //Chat
//		CPacket pakout(0x783);
//		pakout.Add<dword>(entity->_ClientID);
//		pakout.Add<string>(sayStr);
//		server->SendPacketToZone(entity, &pakout);
//	}else if(data->btMsgType == 1){
    //Shout
//		CPacket pakout(0x785);
//		pakout.Add<string>(npcName);
//		pakout.Add<string>(sayStr);
//		server->SendPacketToZone(entity, &pakout);
//	}else if(data->btMsgType == 2){
    //Announce
//		CPacket pakout(0x702);
//		pakout.AddBytes((byte*)npcName, strlen(npcName));
//		pakout.Add<byte>('>');
//		pakout.Add<string>(sayStr);
//		server->SendPacketToZone(entity, &pakout);
//	}
//	delete [] sayStr;
    Log(MSG_DEBUG, "AIACT(028)");
    return AI_SUCCESS;
}

//Unknown
AIACT(029)
{
    //Move within 20% of the distance of my "CALLER"
    GETAIACTDATA(029);
//	if(entity->_EntityType != ENTITY_MONSTER) return AI_FAILURE;
//	CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
//	CWorldEntity* caller = thisMonster->thisZone->GetEntity(thisMonster->_CallerID);
//	if(caller == NULL) return AI_FAILURE;
//	caller->UpdatePosition();
//	thisMonster->UpdatePosition();
    if (!entity->IsMonster( )) return AI_FAILURE;
    CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
    CMap* map = GServer->MapList.Index[thisMonster->Position->Map];
    CCharacter* caller = map->GetCharInMap( thisMonster->owner );
    if (caller == NULL) return AI_FAILURE;
    caller->UpdatePosition();
    thisMonster->UpdatePosition();


//	float fMoveDistance = thisMonster->basic.pos.distance(caller->basic.pos) * 0.2;

//	float m = float(caller->basic.pos.y - thisMonster->basic.pos.y) / float(caller->basic.pos.x - thisMonster->basic.pos.x);
//	float c = (m * thisMonster->basic.pos.x) + thisMonster->basic.pos.y;
//	float y = (m * fMoveDistance) + c;
//	float x = (fMoveDistance - c) / m;
//	thisMonster->stance = 1;
    thisMonster->thisnpc->stance = 1;
    thisMonster->SetStats();
    thisMonster->Position->source=caller->Position->current;
    thisMonster->MoveTo(caller->Position->current);
//	thisMonster->CalculateStats();
//	thisMonster->MoveTo(x, y);

    return AI_SUCCESS;
}

//Do Trigger
AIACT(030)
{
    //word lenszTrigger;	//Pos: 0x00
    //char* szTrigger;	//Pos: 0x02
    GETAIACTDATA(030);
    char* tempName = reinterpret_cast<char*>(&data->szTrigger) - 2;
    dword hash = MakeStrHash(tempName);
    Log(MSG_INFO, "Execute Quest Trigger %s[%d] [%08x]", tempName, data->lenszTrigger, hash);
//	return (entity->ExecuteQuestTrigger(hash) == QUEST_SUCCESS)?AI_SUCCESS:AI_FAILURE;
}

//Unknown
AIACT(031)
{
    //Log(MSG_DEBUG, "AIACT(031)");
    //Attack my "CALLER"(s target) if he is not an ally (must be caller's target...)
//	if(entity->_EntityType != ENTITY_MONSTER) return AI_FAILURE;
//	CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);

//	CWorldEntity* caller = thisMonster->thisZone->GetEntity(thisMonster->_CallerID);
//	if(caller == NULL) return AI_FAILURE;

//	CWorldEntity* target = caller->thisZone->GetEntity(caller->_TargetID);
//	if(target == NULL) return AI_FAILURE;

//	thisMonster->Attack(target);
    if (!entity->IsMonster( )) return AI_FAILURE;
    CMonster* thisMonster = reinterpret_cast<CMonster*>(entity);
    CMap* map = GServer->MapList.Index[thisMonster->Position->Map];
    CCharacter* caller = map->GetCharInMap( thisMonster->owner );
    if (caller == NULL) return AI_FAILURE;
    thisMonster->Position->source=caller->Position->current;
    CCharacter* target = map->GetCharInMap(caller->Battle->target);
    if (target == NULL) return AI_FAILURE;
    thisMonster->StartAction( (CCharacter*) target, NORMAL_ATTACK, 0 );
    //Log(MSG_DEBUG, "AIACT(031)");

    return AI_SUCCESS;
}

//Zone (1)
AIACT(032)
{
    //word nZoneNo;	//Pos: 0x00
    //byte btOnOff;	//Pos: 0x02
    //Set PK Flag (btOnOff) in nZoneNo
    return AI_SUCCESS;
}

//Zone (2)
AIACT(033)
{
    //word nZoneNo //if 0, current map
    //word nNewValue
    //if nNewValue == 2 -> toggle
    //else set regen system nNewValue (0 = off, 1 = on)
    return AI_SUCCESS;
}

//Item (?)
AIACT(034)
{
    //word nItemNum;	//Pos: 0x00
    //word nCount;	//Pos: 0x02
    //Give item to "CALLER"
    Log(MSG_DEBUG, "Item (?)");
    return AI_SUCCESS;
}

//Set Variable (4)
AIACT(035)
{
    //Set AiVAR
    return AI_SUCCESS;
}

//Monster (1)
AIACT(036)
{
    //word nMonster;	//Pos: 0x00
    //byte btMaster;	//Pos: 0x02
    //Spawn nMonster at my position (has btMaster)?
    GETAIACTDATA(036);
//	CWorldEntity* master = NULL;
//	if(data->btMaster) master = entity;

//	entity->thisZone->SpawnMonster(data->nMonster, entity->basic.pos, entity->basic.map, 1, 1000, master);
    Log(MSG_DEBUG, "monster (1)");
    return AI_SUCCESS;
}

//Monster (2)
AIACT(037)
{
    //word nMonster;	//Pos: 0x00
    //word nPos;	//Pos: 0x02
    //dword iDistance;	//Pos: 0x04
    //byte btMaster;	//Pos: 0x08

    //Spawn nMonster at nPos within iDistance (has btMaster)?
    //nPos = 0 my pos
    //nPos = 1 m_pDestCHAR pos
    //nPos = 2 target pos
//	GETAIACTDATA(037);
//	CWorldEntity* master = NULL;
//	if(data->btMaster) master = entity;
//	CVector2F pos;
//	int map;
//	if(data->nPos == 0){
//		entity->UpdatePosition();
//		pos = entity->basic.pos;
//		map = entity->basic.map;
//	}else if(data->nPos == 1){
//		pos = entity->basic.pos;//m_pDestChar
//		map = entity->basic.map;
//	}else if(data->nPos == 2){
//		CWorldEntity* target = entity->thisZone->GetEntity(entity->_TargetID);
//		if(target == NULL) return AI_FAILURE;
//		target->UpdatePosition();
//		pos = target->basic.pos;
//		map = target->basic.map;
//	}

//	entity->thisZone->SpawnMonster(data->nMonster, pos, map, 1, data->iDistance * 100, master);
//Log(MSG_DEBUG, "monster (2)");
    return AI_SUCCESS;
}
