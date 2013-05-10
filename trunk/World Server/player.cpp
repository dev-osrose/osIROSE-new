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
#include "player.h"
#include "worldserver.h"

CPlayer::CPlayer( CClientSocket* CLIENT )
{
    client = CLIENT;
    // USED ITEM
    UsedItem = new USEDITEM;
    assert(UsedItem);
    UsedItem->lastRegTime = 0;
    UsedItem->usevalue = 0;
    UsedItem->usetype = 0;
    UsedItem->userate = 0;
    UsedItem->used = 0;
    // CHARINFO
    CharInfo = new INFO;
    assert(CharInfo);
    memset( &CharInfo->charname, '\0', 17 );
	CharInfo->charid = 0;
    CharInfo->Sex = 0;
    CharInfo->Face = 0;
    CharInfo->Hair = 0;
    CharInfo->Exp = 0;
    CharInfo->Job = 0;
    CharInfo->Zulies = 0;
    CharInfo->Storage_Zulies = 0;
    CharInfo->LastGlobal = 0;
    CharInfo->StatPoints = 0;
    CharInfo->SkillPoints = 0;
    CharInfo->stamina = 0;

    // RIDE
    Ride = new RIDE;
    assert(Ride);
    Ride->Drive = false;
    Ride->Ride = false;
    Ride->charid = 0;
    // TRADE
    Trade = new TRADE;
    assert(Trade);
    for(int i=0;i<10;i++)
        Trade->trade_itemid[i] = 0;
    for(int i=0;i<11;i++)
        Trade->trade_count[i] = 0;
    Trade->trade_status = 0;
    Trade->trade_target = 0;
    //PARTY
    Party = new PARTY;
    assert(Party);
    Party->party = NULL;
    Party->IsMaster = true;
    // SHOP
    Shop = new SHOP;
    assert(Shop);
    Shop->open = false;
    memset( &Shop->name, '\0',64 );
    for(int i=0;i<30;i++)
    {
        ClearItem(Shop->BuyingList[i].item);
        Shop->BuyingList[i].slot = 0;
        Shop->BuyingList[i].count = 0;
        Shop->BuyingList[i].price = 0;
        Shop->SellingList[i].slot = 0;
        Shop->SellingList[i].count = 0;
        Shop->SellingList[i].price = 0;
    }
    Shop->Buying = 0;
    Shop->Selling = 0;
    Shop->ShopType = 0;
    // SESSION
    Session = new SESSION;
    assert(Session);
	Session->userid = 0;
	memset( &Session->username, '\0', 17 );
	memset( &Session->password, '\0', 33 );
	Session->accesslevel = 0;
	Session->isLoggedIn = false;
	Session->inGame = false;
	Session->statdebug = 0;
	// Inventory / storage
    for(unsigned int i=0;i<MAX_INVENTORY;i++)
        ClearItem( items[i] );
    for(unsigned int i=0;i<MAX_STORAGE;i++)
        ClearItem( storageitems[i] );
    // Clan
    Clan = new CLAN;
    assert(Clan);
    Clan->clanid = 0;
    Clan->clanrank = 0;
    Clan->grade = 0;
    Clan->CP = 0;
    Clan->logo = 0;
    Clan->back = 0;
    memset( &Clan->clanname, '\0', 17 );
    //union
    Union_s = new UNION_s;
    // ATTRIBUTES
    Attr = new ATTRIBUTES;
    assert(Attr);
    Attr->Str = 0;
    Attr->Dex = 0;
    Attr->Int = 0;
    Attr->Con = 0;
    Attr->Cha = 0;
    Attr->Sen = 0;
    Attr->Estr = 0;
    Attr->Edex = 0;
    Attr->Eint = 0;
    Attr->Econ = 0;
    Attr->Echa = 0;
    Attr->Esen = 0;

    CharType = TPLAYER;
    questdebug = false;
    Session->codedebug = false;
    Saved = false;
    isInvisibleMode = false;
    Fairy = false;
    //FairyTime = 0;
    nstorageitems = 0;
    p_skills = 0;

    for(int i=0;i<MAX_SKILL;i++)
    {
        cskills[i].id = 0;
        cskills[i].level = 0;
    }

    for(int i=0;i<30;i++){
        bskills[i]   = 0;
        askill[i]    = 0;
        askilllvl[i] = 0;
        pskill[i]    = 0;
        pskilllvl[i] = 0;
    }
    for(int i=0;i<MAX_QUICKBAR;i++)
        quickbar[i] = 0;
    ActiveQuest = 0;
    lastRegenTime = 0;
    lastSaveTime = clock( );
    lastGG = 0;
    VisiblePlayers.clear( );
    VisibleDrops.clear( );
    VisibleMonsters.clear( );
    VisibleNPCs.clear( );
}

CPlayer::~CPlayer( )
{
    if(client!=NULL) delete client;
    if(UsedItem!=NULL) delete UsedItem;
    if(CharInfo!=NULL) delete CharInfo;
    if(Ride!=NULL) delete Ride;
    if(Trade!=NULL) delete Trade;
    if(Party!=NULL) delete Party;
    if(Shop!=NULL) delete Shop;
    if(Session!=NULL) delete Session;
    if(Clan!=NULL) delete Clan;
    if(Attr!=NULL) delete Attr;
    //MyQuest.clear();
    VisiblePlayers.clear();
    VisibleDrops.clear();
    VisibleMonsters.clear();
    VisibleNPCs.clear();
}

bool CPlayer::IsMonster( )
{
    return false;
}

bool CPlayer::UpdateValues( )
{
    if( Ride->Ride && !Ride->Drive )
    {
        CPlayer* otherclient = GServer->GetClientByCID( Ride->charid, Position->Map );
        if( otherclient!=NULL )
        {
            if( otherclient->Status->Stance != 0x04 )
            {
                otherclient->Ride->Drive = false;
                otherclient->Ride->Ride = false;
                otherclient->Ride->charid = 0;
                Ride->Drive = false;
                Ride->Ride = false;
                Ride->charid = 0;
                Position->destiny = Position->current;
            }
            else
            {
                Position->current = otherclient->Position->current;
                Position->destiny = otherclient->Position->destiny;
                Position->lastMoveTime = otherclient->Position->lastMoveTime;
                return false;  // will not update the player position
            }
        }
        else
        {
            Ride->Drive = false;
            Ride->Ride = false;
            Ride->charid = 0;
            Position->destiny = Position->current;
        }
    }
    return true;
}


// Spawn Another User on the Screen
bool CPlayer::SpawnToPlayer(  CPlayer* player, CPlayer* otherclient )
{
/*    if( Party->party!=NULL && Party->party == player->Party->party)
    {
        BEGINPACKET( pak, 0x7d5 );
        ADDDWORD   ( pak, CharInfo->charid );
        ADDWORD    ( pak, clientid );
        ADDWORD    ( pak, GetMaxHP( ) );
        ADDWORD    ( pak, Stats->HP );
        ADDDWORD   ( pak, 0x01000000 );
        ADDDWORD   ( pak, 0x0000000f );
        ADDWORD    ( pak, 0x1388 );
        player->client->SendPacket( &pak );
    }
*/
/*
	if( Ride->Ride )
	{
        CPlayer* rideclient = GServer->GetClientByCID( Ride->charid, Position->Map );
        if(rideclient==NULL)
        {
            Ride->Ride = false;
            Ride->Drive= false;
            Ride->charid = 0;
            return true;
        }
        if( GServer->IsVisible( player, rideclient ) || player->CharInfo->charid == rideclient->CharInfo->charid )
        {
            BEGINPACKET( pak, 0x796 );
            if( Ride->Drive )
            {
                ADDWORD    ( pak, rideclient->clientid );
                ADDFLOAT   ( pak, rideclient->Position->current.x*100 );
                ADDFLOAT   ( pak, rideclient->Position->current.y*100 );
            }
            else
            {
                ADDWORD    ( pak, clientid );
                ADDFLOAT   ( pak, Position->current.x*100 );
                ADDFLOAT   ( pak, Position->current.y*100 );
            }
            ADDWORD    ( pak, 0x0000 );
            player->client->SendPacket( &pak );

            RESETPACKET( pak, 0x7dd );
            ADDBYTE    ( pak, 0x02 );
            if( rideclient->Ride->Drive )
            {
                ADDWORD    ( pak, rideclient->clientid );
                ADDWORD    ( pak, clientid );
            }
            else
            {
                ADDWORD    ( pak, clientid );
                ADDWORD    ( pak, rideclient->clientid );
            }
            player->client->SendPacket( &pak );
        }
    }
	return true;
*/

    CMap* map = GServer->MapList.Index[otherclient->Position->Map];
    BEGINPACKET( pak, 0x793 );
	ADDWORD( pak, clientid);				        // USER ID
    ADDFLOAT( pak, Position->current.x*100 );		// POS X
	ADDFLOAT( pak, Position->current.y*100 );		// POS Y
    ADDFLOAT( pak, Position->destiny.x*100 );		// GOING TO X
    ADDFLOAT( pak, Position->destiny.y*100 );		// GOINT TO Y
	if(Shop->open)
    {
        ADDWORD( pak, 0x000b );
        ADDWORD( pak, 0x0000 );
    }
    else
    if(Stats->HP <= 0)
    {
        ADDWORD( pak, 0x0003 );
        ADDWORD( pak, 0x0000 );
    }
    else
    if(Status->Stance == SITTING)
    {
        ADDWORD( pak, 0x000a );
        ADDWORD( pak, 0x0000 );
    }
    else
    if (Battle->atktarget!=0)
    {
        ADDWORD( pak, 0x0002 );
        ADDWORD( pak, Battle->atktarget );
    }
    else
    if(Position->destiny.x != Position->current.x || Position->destiny.y != Position->current.y)
    {
         ADDWORD( pak, 0x0001 );
         ADDWORD( pak, 0x0000 );
    }
    else
    {
        ADDWORD( pak, 0x0000 );
        ADDWORD( pak, 0x0000 );
    }
    ADDBYTE( pak, Ride->Drive==true?2:1);
	ADDWORD( pak, Stats->HP);
	ADDWORD( pak, 0x0000);

	if(map->allowpvp==1)//clan v clan map
    {
        ADDWORD(pak, otherclient->Clan->clanid );//??
        ADDWORD(pak, 0x0005 );//red icon (map)
    }
    else if(map->allowpvp==2) // pvp
    {
        ADDWORD(pak, otherclient->clientid);
        ADDWORD(pak, 0x0000 );
    }
    else
    {
        ADDWORD(pak, 0x0002 );//white icon (map)
        ADDWORD(pak, 0x0000 );//white icon (map)
    }
	//ADDWORD( pak, 0x00 );
	ADDDWORD( pak, GServer->BuildBuffs( this ) );   // BUFFS
    ADDBYTE( pak, CharInfo->Sex );					// GENDER
    ADDWORD( pak, Stats->Move_Speed );			    // WALK SPEED MAYBE?
	ADDBYTE( pak, 0x00 );										// ??
	ADDBYTE( pak, 0x00 );										// ??
	ADDBYTE( pak, 0x01 );										// ??
    ADDDWORD( pak, CharInfo->Face );				// FACE TYPE
    ADDDWORD( pak, CharInfo->Hair );			    // HAIR TYPE
    ADDDWORD( pak, GServer->BuildItemShow( items[2] ));		// CAP
    ADDDWORD( pak, GServer->BuildItemShow( items[3] ));		// BODY
    ADDDWORD( pak, GServer->BuildItemShow( items[5] ));		// GLOVES
    ADDDWORD( pak, GServer->BuildItemShow( items[6] ));		// BOOTS
    ADDDWORD( pak, GServer->BuildItemShow( items[1] ));		// FACE
    ADDDWORD( pak, GServer->BuildItemShow( items[4] ));		// BACK
    ADDDWORD( pak, GServer->BuildItemShow( items[7] ));		// WEAPON
    ADDDWORD( pak, GServer->BuildItemShow( items[8] ));		// SUBWEAPON
	ADDWORD( pak, GServer->BuildItemHead(items[132] ));
	ADDWORD( pak, GServer->BuildItemHead(items[133] ));
	ADDWORD( pak, GServer->BuildItemHead(items[134] ));
	ADDWORD( pak, CharInfo->Job );
	ADDBYTE( pak, Stats->Level);
	if(items[135].itemnum>0){
    	ADDWORD( pak, (items[135].itemnum +1024));}
	else{
    	ADDWORD( pak, 0x0000 );}
	ADDWORD( pak, 0x0000 );
	if(items[136].itemnum>0){
    	ADDWORD( pak, (items[136].itemnum +1024));}
	else{
    	ADDWORD( pak, 0x0000 );}
	ADDWORD( pak, 0x0000 );
	if(items[137].itemnum>0){
    	ADDWORD( pak, (items[137].itemnum +1024));}
	else{
    	ADDWORD( pak, 0x0000 );}
	ADDWORD( pak, 0x0000 );
	if(items[138].itemnum>0){
    	ADDWORD( pak, (items[138].itemnum +1024));}
	else{
    	ADDWORD( pak, 0x0000 );}
	ADDWORD( pak, 0x0000 );

	ADDWORD( pak, (Stats->HP<=0)?0x0:0xCDCD );
	 if(Shop->open)
    {
        ADDWORD( pak, 0x0002 );
    }
    else
    {
        ADDWORD( pak, 0x0000 );
    }
	ADDWORD( pak, 0x0000 );
	ADDSTRING( pak, CharInfo->charname );
	ADDBYTE( pak, 0x00 );
	if(Status->Dash_up != 0xff)
        ADDWORD ( pak, (MagicStatus[Status->Dash_up].Value));
    if(Status->Haste_up!= 0xff)
        ADDWORD ( pak, (MagicStatus[Status->Haste_up].Value));
	if(Shop->open)
	{
        ADDBYTE( pak, Shop->ShopType);
        ADDBYTE( pak, 0x00);
        ADDSTRING(pak, Shop->name);
        ADDBYTE( pak, 0x00);
    }
    if(Clan->clanid!=0)
    {
        ADDWORD    ( pak, Clan->clanid );
        ADDWORD    ( pak, 0x0000 );
        ADDWORD    ( pak, Clan->back );
        ADDWORD    ( pak, Clan->logo );
        ADDBYTE    ( pak, Clan->grade );
        ADDBYTE    ( pak, Clan->clanrank );
        ADDSTRING  ( pak, Clan->clanname );
    }
    ADDWORD( pak, 0x0000 );
    player->client->SendPacket(&pak);
   	return true;
}
// -----------------------------------------------------------------------------------------
// Returns new skill slot  //bskill askill pskill
// -----------------------------------------------------------------------------------------

int CPlayer::GetNewSkillSlot(char skilltype)
{
	switch (skilltype)
	{
	case 0:
		for(char s=0;s<30;s++)
		{
			if(bskills[s]==0)return s;
		}
	case 1:
		for(char t=0;t<30;t++)
		{
			if(askill[t]==0)return t;
		}
	case 2:
		for(char u=0;u<30;u++)
		{
			if(pskill[u]==0)return u;
		}
	}
	return 0xffff;
}

//Get status of statdebug.
bool CPlayer::GetStatDebug(int statid)
{
    if((Session->statdebug == 99 || Session->statdebug == statid) && statid != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
