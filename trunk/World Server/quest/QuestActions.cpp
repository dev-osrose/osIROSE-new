// Props to ExJam for this code :D Ported to OSpRose by Drakia
#include "../WorldServer.h"

dword GetRewardValue(dword function, dword amount, CPlayer* client, word nDupCNT)
{
    switch (function)
    {
    case 0:
    {
        dword tempVal = amount + 0x1E;
        tempVal *= client->Attr->Cha + 0x0a; // We need to add a way to get all stats totals (Including buff, E, etc) - Drakia
        tempVal *= (100 & 0xFFFF);//World Rate
        tempVal *= 0x14;//Fame + 0x14
        tempVal = ((tempVal / (client->Stats->Level + 0x46)) / 0x7530) + amount;
        return tempVal;
    }
    break;
    case 1:
    {
        dword tempVal = (client->Stats->Level + 3) * amount;
        tempVal *= (client->Attr->Cha >> 1) + client->Stats->Level + 0x28;
        tempVal *= (100 & 0xFFFF);//World Rate
        return tempVal / 0x2710;
    }
    break;
    case 2:
        return (amount * nDupCNT);
        break;
    case 3:
    case 5:
    {
        dword tempVal = amount + 0x14;
        tempVal *= client->Attr->Cha + 0x0a;
        tempVal *= (100 & 0xFFFF);//World Rate
        tempVal *= 0x14;//Fame + 0x14
        tempVal = ((tempVal / (client->Stats->Level + 0x46)) / 0x7530) + amount;
        return tempVal;
    }
    break;
    case 4:
    {
        dword tempVal = amount + 0x2;
        tempVal *= (client->Attr->Cha + client->Stats->Level + 0x28);
        tempVal *= 0x28;//Fame + 0x28
        tempVal *= (100 & 0xFFFF);//World Rate
        return tempVal / 0x222E0;
    }
    break;
    case 6:
    {
        dword tempVal = amount + 0x14;
        tempVal *= client->Attr->Cha + client->Stats->Level;
        tempVal *= 0x14;//Fame + 0x14
        tempVal *= (100 & 0xFFFF);//World Rate
        return (tempVal / 0x2DC6C0) + amount;
    }
    break;
    }
    return amount;
}

//Update Quest
QUESTREWD(000)
{
    GETREWDDATA(000);

    switch (data->btOp) //0 remove, 1 start, 2 replace quest keep items, 3 replace quest delete items, 4 select
    {
    case 0:
    {
        if ( client->questdebug )
            server->SendPM(client, "Remove Quest: %u", data->iQuestSN);
        if (client->ActiveQuest == data->iQuestSN) client->ActiveQuest = 0;
        for (dword i = 0; i < 10; i++)
        {
            if (client->quest.quests[i].QuestID != data->iQuestSN) continue;

            memset(&client->quest.quests[i], 0, sizeof(SQuest));
            break;
        }
    }
    break;
    case 1:
    {
        if ( client->questdebug )
            server->SendPM(client, "Start Quest: %u", data->iQuestSN);
        for (dword i = 0; i < 10; i++)
        {
            if (client->quest.quests[i].QuestID == data->iQuestSN) return QUEST_SUCCESS;
            if (client->quest.quests[i].QuestID != 0) continue;
            memset(&client->quest.quests[i], 0, sizeof(SQuest));
            client->quest.quests[i].QuestID = data->iQuestSN;
            client->quest.quests[i].StartTime = time(NULL);
            break;
        }
        client->ActiveQuest = data->iQuestSN;
    }
    break;
    case 2:
    {
        if ( client->questdebug )
            server->SendPM(client, "Replace Quest, keep items: %u", data->iQuestSN);
        for (dword i = 0; i < 10; i++)
        {
            if (client->quest.quests[i].QuestID != client->ActiveQuest) continue;
            client->quest.quests[i].QuestID = data->iQuestSN;
            client->quest.quests[i].StartTime = time(NULL);
            break;
        }
        client->ActiveQuest = data->iQuestSN;
    }
    break;
    case 3:
    {
        if ( client->questdebug )
            server->SendPM(client, "Replace Quest, delete items: %u", data->iQuestSN);
        for (dword i = 0; i < 10; i++)
        {
            if (client->quest.quests[i].QuestID != client->ActiveQuest) continue;
            memset(&client->quest.quests[i], 0, sizeof(SQuest));
            client->quest.quests[i].QuestID = data->iQuestSN;
            client->quest.quests[i].StartTime = time(NULL);
            break;
        }
        client->ActiveQuest = data->iQuestSN;
    }
    break;
    case 4:
    {
        if ( client->questdebug )
            server->SendPM(client, "Select Quest: %u", data->iQuestSN);
        client->ActiveQuest = data->iQuestSN;
    }
    break;
    }
    return QUEST_SUCCESS;
}

//Update Quest Items
QUESTREWD(001)
{
    GETREWDDATA(001);
    if ( client->questdebug )
        server->SendPM(client, "Update Quest Item (uiItemSN: %u btOp: %u nDupCNT: %u)", data->uiItemSN, data->btOp, data->nDupCNT);
    CItem tmpItem;
    tmpItem.itemtype = data->uiItemSN / 1000;
    tmpItem.itemnum = data->uiItemSN % 1000;
    tmpItem.count = data->nDupCNT;

    SQuest* curQuest = client->GetActiveQuest();
    if (curQuest == NULL)
    {
        Log(MSG_DEBUG, "Couldn't find active quest, wtf? Id %u", client->ActiveQuest);
        return QUEST_FAILURE;
    }
    curQuest->AddItem(&tmpItem, data->btOp);

    return QUEST_SUCCESS;
}

//Set Quest Variable
QUESTREWD(002)
{
    GETREWDDATA(002);
    for (dword i = 0; i < data->iDataCnt; i++)
    {
        dword address = i * sizeof(STR_QUEST_DATA);
        address += (dword)data;
        address += 4;
        STR_QUEST_DATA* curQst = (STR_QUEST_DATA*)address;
        if ( client->questdebug )
            server->SendPM(client, "Set quest var[%#04x][%i] - %i (Op: %i)", curQst->m_wVarTYPE, curQst->m_wVarNO, curQst->nValue, curQst->btOp );

        word nValue = curQst->nValue;
        word tempValue = client->GetQuestVar(curQst->m_wVarTYPE, curQst->m_wVarNO);
        OperateValues<word>(curQst->btOp, &tempValue, curQst->nValue);
        client->SetQuestVar(curQst->m_wVarTYPE, curQst->m_wVarNO, tempValue);
    }

    return QUEST_SUCCESS;
}

//Update Stats
QUESTREWD(003)
{
    GETREWDDATA(003);
    for (dword i = 0; i < data->iDataCnt; i++)
    {
        dword address = i * 0x0C;
        address += (dword)data;
        address += 4;
        STR_ABIL_DATA* curAbil = (STR_ABIL_DATA*)address;
        BEGINPACKET(pak, 0x721 );
        if ( client->questdebug )
            server->SendPM(client, "Update stat %i - %i (Op: %i)", curAbil->iType, curAbil->iValue, curAbil->btOp);

        switch ( curAbil->iType )
        {
        case sGender:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->CharInfo->Sex, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sJob:
        {
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->CharInfo->Job, curAbil->iValue))
                return QUEST_FAILURE;
            RESETPACKET(pak, 0x721 );
            ADDWORD(pak, curAbil->iType );
            ADDDWORD(pak, curAbil->iValue );
            client->client->SendPacket( &pak );
        }
        break;

        case sUnion:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Union_s->unionvar[0], curAbil->iValue))
                return QUEST_FAILURE;
            RESETPACKET(pak, 0x721 );
            ADDWORD(pak, curAbil->iType );
            ADDDWORD(pak, curAbil->iValue );
            client->client->SendPacket( &pak );
            // We actually don't have union code.
            break;

        case sUnionPoints:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Union_s->unionvar[client->Union_s->unionvar[0]], curAbil->iValue))
                return QUEST_FAILURE;
            RESETPACKET(pak, 0x720 );
            ADDWORD(pak, curAbil->iType );
            ADDDWORD(pak, curAbil->iValue );
            client->client->SendPacket( &pak );
            break;

        case sStrength:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Attr->Str, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sDexterity:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Attr->Dex, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sIntelligence:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Attr->Int, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sConcentration:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Attr->Con, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sCharm:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Attr->Cha, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sSensibility:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Attr->Sen, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sLevel:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->Stats->Level, curAbil->iValue))
                return QUEST_FAILURE;
            break;

        case sStatPoints:
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->CharInfo->StatPoints, curAbil->iValue))
                return QUEST_FAILURE;

            // FJMK -- Begin Code
            // May 2013
            // Quest Rewards Statpoins Implementation
            RESETPACKET( pak, 0x720 );
            ADDWORD( pak, 32 );
            ADDWORD( pak, curAbil->iValue );
            ADDWORD( pak, 0 );
            client->client->SendPacket( &pak );

            server->SendPM(client, "[Quest Reward] You reveived %i statpoints.", curAbil->iValue);
            // FJMK -- End Code
            break;
        case sMoney:
        {
            if (!OperateValues<int>(curAbil->btOp, (int*)&client->CharInfo->Zulies, curAbil->iValue))
                return QUEST_FAILURE;
            // Send zuly update packet
            //	BEGINPACKET(pak, 0x720 );
            //	ADDWORD(pak, curAbil->iType );
            //	ADDDWORD(pak, curAbil->iValue );
            //	client->client->SendPacket( &pak );
            BEGINPACKET(pak, 0x71D);
            ADDQWORD(pak, client->CharInfo->Zulies);
            client->client->SendPacket(&pak);
            break;
        }
        default:
            Log(MSG_WARNING, "Type Unknown: '%i'", curAbil->iType);
            break;
        }
    }
    return QUEST_SUCCESS;
}

//Set Quest Variable
QUESTREWD(004)
{
    return QUEST_REWD_002(server, client, raw);
}

//Give Reward
QUESTREWD(005)
{
    GETREWDDATA(005);
    switch (data->btTarget)
    {
    case 0://EXP
    {
        if ( client->questdebug )
            server->SendPM(client, "Give EXP: %i", data->iValue);
        client->CharInfo->Exp += GetRewardValue(data->btEquation, data->iValue, client, 0);
        BEGINPACKET(pak, 0x79b);
        ADDDWORD(pak, client->CharInfo->Exp);
        ADDDWORD(pak, client->CharInfo->stamina);
        ADDWORD (pak, 0);
        client->client->SendPacket(&pak);
    }
    break;
    case 1://Zuly
    {
        if ( client->questdebug )
            server->SendPM(client, "Give Zuly: %i", data->iValue);
        client->CharInfo->Zulies += GetRewardValue(data->btEquation, data->iValue, client, 1);//dunno nDupCount for this one!
        BEGINPACKET(pak, 0x71D);
        ADDQWORD(pak, client->CharInfo->Zulies);
        client->client->SendPacket(&pak);
    }
    break;
    case 2://Item
    {
        CItem nItem;
        nItem.itemtype = data->iItemSN / 1000;
        nItem.itemnum = data->iItemSN % 1000;
        if (nItem.IsStackable())
        {
            nItem.count = GetRewardValue(data->btEquation, data->iValue, client, 0);
        }
        else
            nItem.count = 1;

        if ( client->questdebug )
            server->SendPM(client, "Give item [%i][%i]x%i", nItem.itemtype, nItem.itemnum, nItem.count);

        nItem.durability = GServer->STB_ITEM[nItem.itemtype-1].rows[nItem.itemnum][29];
        nItem.gem = 0;
        nItem.stats = 0;
        nItem.refine = 0;
        nItem.socketed = 0;
        nItem.lifespan = 100;
        nItem.appraised = 1;
        dword slot = client->AddItem(nItem);
        if (slot == 0xffff)   // Fail
        {
            BEGINPACKET( pak, 0x7a7);
            ADDWORD(pak, 0x00);
            ADDBYTE(pak, 0x03);
            ADDBYTE(pak, 0x00);
            client->client->SendPacket(&pak);
        }
        else   // Success
        {
            BEGINPACKET( pak, 0x71f);
            ADDBYTE(pak, 0x01);
            ADDBYTE(pak, slot);
            ADDWORD(pak, client->items[slot].GetPakHeader( ) );
            ADDDWORD(pak, client->items[slot].GetPakData( ) );
            client->client->SendPacket(&pak);
        }
    }
    break;
    }

    return QUEST_SUCCESS;
}

//Restore HP/MP
QUESTREWD(006)
{
    GETREWDDATA(006);
    if ( client->questdebug )
        server->SendPM(client, "Restore %i%% HP, %i%% MP", data->iPercentOfHP, data->iPercentOfMP);
    client->Stats->HP = (long int)((float)client->Stats->MaxHP / 100.0f) * data->iPercentOfHP;
    client->Stats->MP = (long int)((float)client->Stats->MaxHP / 100.0f) * data->iPercentOfMP;

    return QUEST_SUCCESS;
}

//Teleport
QUESTREWD(007)
{
    GETREWDDATA(007);
    fPoint thispoint;
    thispoint.x = floor(((float)data->iX)/100);
    thispoint.y = floor(((float)data->iY)/100);
    if ( client->questdebug )
        server->SendPM(client, "Teleport [%i][%f][%f]", data->iZoneSN, thispoint.x, thispoint.y);
    GServer->TeleportTo(client, data->iZoneSN, thispoint);
    return QUEST_SUCCESS;
}

//Spawn Monster
QUESTREWD(008)
{
    GETREWDDATA(008);

    fPoint position;
    dword mapId;
    if (data->iX == 0 || data->iY == 0 || data->iZoneSN == 0)
    {
        position.x = client->Position->current.x;
        position.y = client->Position->current.y;
        mapId = client->Position->Map;
    }
    else
    {
        position.x = data->iX / 100;
        position.y = data->iY / 100;
        mapId = data->iZoneSN;
    }
    for (dword i = 0; i < data->iHowMany; i++)
    {
        fPoint pos = GServer->RandInCircle( position, data->iRange );
        if ( client->questdebug )
            server->SendPM(client, "Spawn mob[%i] @ %f, %f", data->iMonsterSN, pos.x, pos.y);
        CMap* map = GServer->MapList.Index[mapId];
        CMonster* mon = map->AddMonster( data->iMonsterSN, pos, 0, NULL, NULL, 0 , true );
        if (data->iMonsterSN > 750 && data->iMonsterSN < 755) // ghost rackies = non aggresive
        {
            mon->thisnpc->aggresive = 0;
        }
        else
        {
            mon->thisnpc->aggresive = 999; // Force the mob to be agressive.
        }
        mon->lastSighCheck = 0; // Force sight check instantly.
    }
    return QUEST_SUCCESS;
}

//Execute Quest Trigger
QUESTREWD(009)
{
    GETREWDDATA(009);
    char* tempName = reinterpret_cast<char*>(&data->szNextTriggerSN) - 2;
    dword hash = MakeStrHash(tempName);
    if ( client->questdebug )
        server->SendPM(client, "Execute Quest Trigger %s[%d] [%08x]", tempName, data->shNameLen, hash);
    return client->ExecuteQuestTrigger(hash);
    return QUEST_SUCCESS;
}

//Reset Stats
QUESTREWD(010)
{
    if ( client->questdebug )
        server->SendPM(client, "Reset Stats");
    client->CharInfo->StatPoints = 0;

    client->Attr->Str = 15;
    client->Attr->Dex = 15;
    client->Attr->Int = 15;
    client->Attr->Con = 15;
    client->Attr->Cha = 10;
    client->Attr->Sen = 10;

    for (int i = 2; i <= client->Stats->Level; i++)
    {
        client->CharInfo->StatPoints += 10;
        client->CharInfo->StatPoints += i / 2;
    }

    return QUEST_SUCCESS;
}

//Update Object Var
QUESTREWD(011)
{
    GETREWDDATA(011);
    /*if(entity->_EntityType != ENTITY_NPC) return QUEST_FAILURE;

    if(data->btWho == 0){//Npc
    	CNpc* thisNpc = reinterpret_cast<CNpc*>(entity);
    	thisNpc = thisNpc->SelectedNpc;
    	if(thisNpc == NULL) return QUEST_FAILURE;

    	short VarValue = thisNpc->ObjVar.GetVar(data->nVarNo);
    	OperateValues(data->btOp, &VarValue, (short)data->iValue);
    	thisNpc->ObjVar.SetVar(data->nVarNo, VarValue);
    }else if(data->btWho == 1){//Event
    	short VarValue = server->EventVar.GetVar(data->nVarNo);
    	OperateValues(data->btOp, &VarValue, (short)data->iValue);
    	server->EventVar.SetVar(data->nVarNo, VarValue);
    }

    return QUEST_SUCCESS;*/
    return QUEST_FAILURE; // We can't do this yet I don't think? - Drakia
}

//NPC Speak
QUESTREWD(012)
{
    /*if(entity->_EntityType != ENTITY_NPC) return QUEST_FAILURE;
    GETREWDDATA(012);

    CNpc* thisNpc = reinterpret_cast<CNpc*>(entity);
    if(thisNpc->SelectedNpc == NULL) return QUEST_FAILURE;
    std::map<dword, char*>::iterator triggerITR = server->NpcNames.find(thisNpc->SelectedNpc->NpcID);
    if(triggerITR == server->NpcNames.end()) return QUEST_FAILURE;

    char* npcName = triggerITR->second;
    char* sayStr = server->lngQst->GetLTBStr(data->iStrID);
    if(sayStr == NULL) return QUEST_FAILURE;
    if(data->btMsgType == 1){
    	//Shout
    	CPacket pakout(0x785);
    	pakout.Add<string>(npcName);
    	pakout.Add<string>(sayStr);
    	server->SendPacketToZone(thisNpc, &pakout);
    }else if(data->btMsgType == 2){
    	//Announce
    	CPacket pakout(0x702);
    	pakout.AddBytes((byte*)npcName, strlen(npcName));
    	pakout.Add<byte>('>');
    	pakout.Add<string>(sayStr);
    	server->SendPacketToZone(thisNpc, &pakout);
    }
    delete [] sayStr;

    return QUEST_SUCCESS;*/
    return QUEST_FAILURE; // This is cool and all, but we lack the stuff to do it - Drakia
}

//Unknown
QUESTREWD(013)
{
    return QUEST_SUCCESS;
}

//Learn Skill
QUESTREWD(014)
{
    GETREWDDATA(014);
    if ( client->questdebug )
        server->SendPM(client, "Learn skill: %i", data->iSkillNo);
    GServer->LearnSkill(client, data->iSkillNo, false);
    return QUEST_SUCCESS;
}

//Set Quest Flag
QUESTREWD(015)
{
    GETREWDDATA(015);
    if ( client->questdebug )
        server->SendPM(client, "Set QFlag[%i]=%i", data->nSN, data->btOp);
    client->quest.SetFlag(data->nSN, (data->btOp == 1)?true:false);
    return QUEST_SUCCESS;
}

//Unknown
QUESTREWD(016)
{
    return QUEST_SUCCESS;
}

//Reset All Quest Flags
QUESTREWD(017)
{
    if ( client->questdebug )
        server->SendPM(client, "Reset all quest flags");
    memset(&client->quest.flags, 0, 64);
    return QUEST_SUCCESS;
}

//Send Announcement
QUESTREWD(018)
{
    GETREWDDATA(018);
    return QUEST_SUCCESS;
}

//Execute Quest Trigger in Other Map
QUESTREWD(019)
{
    return QUEST_SUCCESS;
}

//PvP Status
QUESTREWD(020)
{
    return QUEST_SUCCESS;
}

//Set Respawn Position
QUESTREWD(021)
{
    return QUEST_SUCCESS;
}

//Unknown
QUESTREWD(022)
{
    return QUEST_SUCCESS;
}

//Raise Clan Grade - This is not the least bit efficient - Drakia
QUESTREWD(023)
{
    GETREWDDATA(023);
    if ( client->questdebug )
        server->SendPM(client, "Raise clan grade");
    for (UINT i=0;i<GServer->MapList.Map.size();i++)
    {
        CMap* map = GServer->MapList.Map.at(i);
        if ( map->PlayerList.size()<1 )
            continue;
        for (UINT j=0;j<map->PlayerList.size();j++)
        {
            CPlayer* player = map->PlayerList.at(j);
            if ( player->Clan->clanid != client->Clan->clanid ) continue;
            player->Clan->grade++;

            //load clan info in char server
            BEGINPACKET( pak, 0x7e0 );
            ADDBYTE    ( pak, 0xfb ); //action to update clan informacion (charserver)
            ADDWORD    ( pak, player->Clan->clanid );
            ADDWORD    ( pak, player->Clan->grade );
            GServer->SendISCPacket( &pak );

            //Send to other players
            RESETPACKET( pak, 0x7e0 );
            ADDBYTE    ( pak, 0x35 );
            ADDWORD    ( pak, player->clientid );
            ADDWORD    ( pak, player->Clan->clanid);
            ADDWORD    ( pak, 0x0000 );//???
            ADDWORD    ( pak, player->Clan->back );
            ADDWORD    ( pak, player->Clan->logo );
            ADDBYTE    ( pak, player->Clan->grade );//clan grade
            ADDBYTE    ( pak, 0x06 );//clan rank
            ADDSTRING  ( pak, player->Clan->clanname );
            ADDBYTE    ( pak, 0x00 );
            GServer->SendToVisible( &pak, player );
        }
    }
    GServer->DB->QExecute("UPDATE list_clan SET grade=%i WHERE id=%i", client->Clan->grade, client->Clan->clanid);
    return QUEST_SUCCESS;
}

//Clan Money
QUESTREWD(024)
{
    return QUEST_SUCCESS;
}

//Clan Points
QUESTREWD(025)
{
    return QUEST_SUCCESS;
}

//Clan Skill
QUESTREWD(026)
{
    return QUEST_SUCCESS;
}

//Clan Contribution
QUESTREWD(027)
{
    return QUEST_SUCCESS;
}

//Clan Teleportation - Again, not efficient. Maybe keep a list of pointers to clan members? - Drakia
QUESTREWD(028)
{
    GETREWDDATA(028);
    fPoint telepos;
    fPoint newPos;
    telepos.x = data->iX/100;
    telepos.y = data->iY/100;
    if ( client->questdebug )
        server->SendPM(client, "Clan teleport to [%i][%f][%f]", data->nZoneNo, telepos.x, telepos.y);
    for (UINT i=0;i<GServer->MapList.Map.size();i++)
    {
        CMap* map = GServer->MapList.Map.at(i);
        if ( map->PlayerList.size()<1 )
            continue;
        for (UINT j=0;j<map->PlayerList.size();j++)
        {
            CPlayer* player = map->PlayerList.at(j);
            if ( player->Clan->clanid != client->Clan->clanid ) continue;
            newPos = GServer->RandInCircle(telepos, data->iRange);
            GServer->TeleportTo(player, data->nZoneNo, newPos );
        }
    }
    return QUEST_SUCCESS;
}
