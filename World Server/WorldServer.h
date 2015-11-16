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

    developed with Main erose/hrose source server
     + some change from the original eich source
     + parts copied from/based on work from ExJem
*/
//#define debugging
#ifndef __ROSE_SERVERS__
#define __ROSE_SERVERS__
#include "../Common/sockets.h"
#include "worldmonster.h"
#include "quest/CRoseArray.hpp"
#include "quest/CRoseFile.hpp"
#include "quest/CStrStb.hpp"
#include "quest/CIntStb.hpp"
#include "quest/QuestTrigger.h"
#include "aip/Aip.h"
#include "worldmap.h"
#include "player.h"
#include "character.h"
#include "party.h"
#include "datatypes.h"
#include "stbhandler.h"
#include "strhashing.h"
#include "math.h"

#define MAXVISUALRANGE 90
#define MINVISUALRANGE 80
#define ClearItem( i )        \
	{                         \
		i.appraised  = false; \
		i.count      = 0;     \
		i.durability = 0;     \
		i.itemnum    = 0;     \
		i.itemtype   = 0;     \
		i.lifespan   = 0;     \
		i.refine     = 0;     \
		i.socketed   = false; \
		i.stats      = 0;     \
		i.gem        = 0;     \
	}

#define WORLD_THREAD 0
#define VISUALITY_THREAD 1
#define SHUTDOWN_THREAD 2

#define MAX_MAPS 115

// Main loginserver server class
class CWorldServer : public CServerSocket
{
	public:
	//------------------ MAIN (worldserver.cpp)
	CWorldServer( string );
	CClientSocket* CreateClientSocket( );
	~CWorldServer( );
	bool OnReceivePacket( CClientSocket* thisclient, CPacket* P );
	void ReceivedISCPacket( CPacket* pak ); // Raven0123
	void SendISCServerInfo( );
	void OnClientDisconnect( CClientSocket* thisclient );
	void DeleteClientSocket( CClientSocket* thisclient );
	bool OnServerReady( );
	void SpawnMonsters( );
	bool Ping( );
	void LoadConfigurations( char* );
	void LoadCommandLevels( void );
	void ServerLoop( );
	void LoadEncryption( );
	bool pakGiveItem( CPlayer* thisclient, unsigned char itemtype, unsigned short itemid, unsigned short count );
	bool pakGiveExp( CPlayer* thisclient, unsigned exp );
	bool pakGiveMoney( CPlayer* thisclient, unsigned amount );
	bool pakTeleport( CPlayer* thisclient, unsigned short map, float x, float y );
	bool pakLearnSkill( CPlayer* thisclient, unsigned short skillid );
	void DePopulateZone( CPlayer* thisclient, int zonex, int zoney );
	bool pakQuestTrigger( CPlayer* thisclient, CPacket* pak );

	//------------------ WORLD PROCESS (worldprocess.cpp)
	bool GiveExp( CMonster* thismon );

	//------------------ BUFFS (buff.cpp)
	CBValue GetBuffValue( CSkills* thisskill, CCharacter* character, uint32_t Evalue, uint32_t i, uint32_t up, uint32_t down, uint32_t CurrentValue, bool Buff = true, bool Status = false );
	//CBValue GetBuffValue( CSkills* thisskill, CCharacter* character, uint32_t Evalue, uint32_t i, uint32_t up, uint32_t down, uint32_t CurrentValue, bool Buff=true );
	bool CheckABuffs( CSkills* thisskill, CCharacter* character, int Evalue, int i );
	bool CheckDBuffs( CSkills* thisskill, CCharacter* character, int Evalue, int i );
	bool AddBuffs( CSkills* thisskill, CCharacter* character, int Evalue, bool flag );
	bool AddDeBuffs( CSkills* thisskill, CCharacter* character, int Evalue );
	bool AddBuffs( CSkills* thisskill, CCharacter* character, int Evalue );

	//------------------ SERVER EXTRAS (extrafunctions.cpp)
	bool     IsValidItem( uint32_t type, uint32_t id );
	unsigned BuildItemShow( CItem thisitem );
	uint32_t GetUIntValue( const char* s, void* var = NULL );
	int      GetIntValue( const char* s, void* var = NULL );
	char*    GetStrValue( const char* s, void* var = NULL );
	bool     SendSysMsg( CPlayer* thisclient, string message );
	uint32_t RandNumber( uint32_t init, uint32_t range, uint32_t seed = 0 );
	uint32_t GetColorExp( uint32_t playerlevel, uint32_t moblevel, uint32_t exp );
	bool     CheckInventorySlot( CPlayer* thisclient, int slot );
	bool     pakGMClass( CPlayer* thisclient, char* classid );
	bool     pakGMKillInRange( CPlayer* thisclient, int range );
	bool     pakGMHide( CPlayer* thisclient, int mode );
	void     SendToVisible( CPacket* pak, CPlayer* thisclient, bool thisclients = true );
	void     SendToVisible( CPacket* pak, CPlayer* thisclient, CPlayer* xotherclient );

	void SendToVisible( CPacket* pak, class CCharacter* character, CDrop* thisdrop = NULL );

	bool           IsMonInCircle( CPlayer* thisclient, CMonster* thismon, float radius );
	bool           IsPlayerInCircle( CPlayer* thisclient, CPlayer* otherclient, float radius );
	CUseInfo*      GetUseItemInfo( CPlayer* thisclient, uint32_t slot );
	bool           IsVisible( CPlayer* thisclient, CPlayer* otherclient );
	bool           IsMonInCircle( fPoint center, fPoint position, float radius );
	bool           IsVisible( CPlayer* thisclient, CMonster* thismon );
	bool           IsVisible( CPlayer* thisclient, CDrop* thisdrop );
	CItem          GetItemByHeadAndData( unsigned head, unsigned data );
	bool           IsVisible( CPlayer* thisclient, CNPC* thisnpc );
	void           SendToVisible( CPacket* pak, CMonster* thismon, CDrop* thisdrop = NULL );
	void           SendToVisible( CPacket* pak, CDrop* thisdrop );
	CPlayer*       GetClientByUserName( char* username );
	uint32_t       BuildBuffs( CCharacter* player );
	uint32_t       BuildUpBuffs( CCharacter* player );
	uint32_t       BuildDeBuffs( CCharacter* player );
	void           CheckForLevelUp( CPlayer* thisclient );
	bool           isSkillTargetFriendly( CSkills* thisskill );
	CRespawnPoint* GetRespawnByID( uint32_t id );
	CPlayer*       GetClientByCID( uint32_t id, uint32_t map = 0 );
	CPlayer*       GetClientByCharName( char* name );
	CPlayer*       GetClientByID( uint32_t id, uint32_t map = 0 );
	CTeleGate*     GetTeleGateByID( uint32_t id );
	uint32_t       BuildItemHead( CItem thisitem );
	uint32_t       BuildItemData( CItem thisitem );
	CMonster*      GetMonsterByID( uint32_t id, uint32_t map );
	CSpawnArea*    GetSpawnArea( uint32_t id, uint32_t map = 0 );
#ifdef USEIFO
	CMobGroup* GetMobGroup( uint32_t id, uint32_t map = 0 );
#endif
	bool            DeleteSpawn( CSpawnArea* spawn );
	CNPCData*       GetNPCDataByID( uint32_t id );
	void            SendToMap( CPacket* pak, int mapid );
	unsigned        BuildItemRefine( CItem thisitem );
	CMDrops*        GetDropData( uint32_t id );
	CRespawnPoint*  GetRespawnByMap( int id );
	CSkills*        GetSkillByID( uint32_t id );
	CStatus*        GetStatusByID( uint32_t id );
	void            ClearClientID( uint32_t id );
	CDrop*          GetDropByID( uint32_t id, uint32_t map = 0 );
	CNPC*           GetNPCByID( uint32_t id, uint32_t map = 0 );
	CDrop*          GetDrop( CMonster* thismon );
	CDrop*          GetPYDrop( CMonster* thismon, uint32_t droptype );
	void            SendToAll( CPacket* pak );
	uint32_t        GetNewClientID( );
	void            DisconnectAll( );
	CPlayer*        GetClientByUserID( uint32_t userid );
	uint32_t        GetLevelGhost( uint32_t map, uint32_t level );
	uint32_t        GetFairyRange( uint32_t part );
	void            RefreshFairy( );
	inline uint32_t GetStorageFee( uint32_t iBasePrice, uint32_t iPriceRate, uint32_t uiDupCnt )
	{
		return ( ( ( ( iBasePrice * 5 ) / 0x3E8 ) + 1 ) * ( iPriceRate + 1 ) ) * uiDupCnt;
	};

	//------------------ Fairies ---------------------
	void DoFairyStuff( CPlayer* targetclient, int action );
	void DoFairyFree( int fairy );

	// Repair Price (extrafunctions.cpp)
	LONG Repairprice( uint32_t price, uint32_t durability, uint32_t lifespan );
	//------------------ MATH (extrafunctions.cpp)
	fPoint RandInCircle( fPoint center, float radius );
	fPoint RandInPoly( fPoint p[], int pcount );
	fPoint RandInTriangle( fPoint p1, fPoint p2, fPoint p3 );
	float  AreaOfTriangle( fPoint p1, fPoint p2, fPoint p3 );
	float  distance( fPoint pos1, fPoint pos2 );

	//------------------ PACKETS (worldpackets.cpp)
	bool pakItemMall( CPlayer* thisclient, CPacket* P );
	bool pakWeight( CPlayer* thisclient, CPacket* P );
	bool pakPrintscreen( CPlayer* thisclient, CPacket* P );
	bool pakModifiedItemDone( CPlayer* thisclient, CPacket* P );
	bool pakSpawnDrop( CPlayer* thisclient, CDrop* thisdrop );
	bool pakChangeStorage( CPlayer* thisclient, CPacket* P );
	bool pakModifiedItem( CPlayer* thisclient, CPacket* P );
	bool pakPartyManager( CPlayer* thisclient, CPacket* P );
	bool pakLevelUpSkill( CPlayer* thisclient, CPacket* P );
	bool pakChangeStance( CPlayer* thisclient, CPacket* P );
	bool pakCharDSClient( CPlayer* thisclient, CPacket* P );
	bool pakChangeEquip( CPlayer* thisclient, CPacket* P );
	bool pakStartAttack( CPlayer* thisclient, CPacket* P );
	bool pakSpawnNPC( CPlayer* thisclient, CNPC* thisnpc );
	bool pakRideRequest( CPlayer* thisclient, CPacket* P );
	bool pakPartyActions( CPlayer* thisclient, CPacket* P );
	bool pakPartyOption( CPlayer* thisclient, CPacket* P );
	bool pakTradeAction( CPlayer* thisclient, CPacket* P );
	bool pakChangeCart( CPlayer* thisclient, CPacket* P );
	bool pakSkillSelf( CPlayer* thisclient, CPacket* P );
	bool pakDoIdentify( CPlayer* thisclient, CPacket* P );
	bool pakNormalChat( CPlayer* thisclient, CPacket* P );
	bool pakCharSelect( CPlayer* thisclient, CPacket* P );
	bool pakStartSkill( CPlayer* thisclient, CPacket* P );
	bool pakRepairHammer( CPlayer* thisclient, CPacket* P );
	bool pakRepairItem( CPlayer* thisclient, CPacket* P );
	bool pakMoveSkill( CPlayer* thisclient, CPacket* P );
	bool pakCloseShop( CPlayer* thisclient, CPacket* P );
	bool pakStoreZuly( CPlayer* thisclient, CPacket* P );
	bool pakPartyChat( CPlayer* thisclient, CPacket* P );
	bool pakGameGuard( CPlayer* thisclient, CPacket* P );
	bool pakOpenShop( CPlayer* thisclient, CPacket* P );
	bool pakShowShop( CPlayer* thisclient, CPacket* P );
	bool pakSellShop( CPlayer* thisclient, CPacket* P );
	bool pakMoveChar( CPlayer* thisclient, CPacket* P );
	bool pakStopChar( CPlayer* thisclient, CPacket* P );
	bool pakPickDrop( CPlayer* thisclient, CPacket* P );
	bool pakAddStats( CPlayer* thisclient, CPacket* P );
	bool pakShowHeal( CPlayer* thisclient, CPacket* P );
	bool pakSkillAOE( CPlayer* thisclient, CPacket* P );
	bool pakidentify( CPlayer* thisclient, CPacket* P );
	bool pakUserDied( CPlayer* thisclient, CPacket* P );
	bool pakSaveTown( CPlayer* thisclient, CPacket* P );
	bool pakTradeAdd( CPlayer* thisclient, CPacket* P );
	bool pakUseItem( CPlayer* thisclient, CPacket* P );
	bool pakWhisper( CPlayer* thisclient, CPacket* P );
	bool pakEquipABC( CPlayer* thisclient, CPacket* P );
	bool pakStorage( CPlayer* thisclient, CPacket* P );
	bool pakBuyShop( CPlayer* thisclient, CPacket* P );
	bool pakDoEmote( CPlayer* thisclient, CPacket* P );
	bool pakNPCBuy( CPlayer* thisclient, CPacket* P );
	bool pakDoDrop( CPlayer* thisclient, CPacket* P );
	bool pakShout( CPlayer* thisclient, CPacket* P );
	bool pakCraft( CPlayer* thisclient, CPacket* P );
	bool pakDoID( CPlayer* thisclient, CPacket* P );
	bool pakGate( CPlayer* thisclient, CPacket* P );
	bool pakExit( CPlayer* thisclient, CPacket* P );
	bool pakPing( CPlayer* thisclient, CPacket* P );
	bool SendLevelUPtoChar( CPlayer* thisclient );
	void pakPlayer( CPlayer* thisclient );
	void pakInventory( CPlayer* thisclient );
	void pakQuestData( CPlayer* thisclient );
	void pakClearUser( CPlayer* thisclient );
	bool LearnSkill( CPlayer* thisclient, uint32_t skill, bool cost = true );

	//-------------------------- Clan functions
	bool pakClanManager( CPlayer* thisclient, CPacket* P );
	bool pakCreateClan( CPlayer* thisclient, CPacket* P );

	//------------------ GM COMMANDS (gmcmds.cpp)
	bool pakGMMoveTo( CPlayer* thisclient, fPoint position );
	bool pakGMTeleAllHere( CPlayer* thisclient );
	bool GMShowTargetInfo( CPlayer* thisclient );
	bool pakGMServerInfo( CPlayer* thisclient );
	bool pakGMHeal( CPlayer* thisclient );
	bool pakGMStat( CPlayer* thisclient, char* statname, int statvalue );
	bool pakGMItemtoplayer( CPlayer* thisclient, char* name, uint32_t itemid, uint32_t itemtype, uint32_t itemamount, uint32_t itemrefine, uint32_t itemls, uint32_t itemstats, uint32_t itemsocket );
	bool pakGMItem( CPlayer* thisclient, uint32_t itemid, uint32_t itemtype, uint32_t itemamount, uint32_t itemrefine, uint32_t itemls, uint32_t itemstats, uint32_t itemsocket );
	bool pakGMTeleOtherPlayer( CPlayer* thisclient, char* name, int map, float x, float y );
	bool pakGMZuly( CPlayer* thisclient, int mode, int amount, char* charname );
	bool pakGMTele( CPlayer* thisclient, int map, float x, float y );
	bool pakGMMon( CPlayer* thisclient, int montype, int moncount );
	bool pakGMZulygive( CPlayer* thisclient, char* name, int zuly );
	bool pakGMFairyto( CPlayer* thisclient, char* name, int mode );
	bool pakGMManageFairy( CPlayer* thisclient, int mode );
	bool pakGMChangeFairyWait( CPlayer* thisclient, int newvalue );
	bool pakGMChangeFairyStay( CPlayer* thisclient, int newvalue );
	bool pakGMChangeFairyTestMode( CPlayer* thisclient, int mode );
	bool pakGMTelePlayerHere( CPlayer* thisclient, char* name );
	bool pakGMLevel( CPlayer* thisclient, int level, char* name );
	bool pakGMTeleToPlayer( CPlayer* thisclient, char* name );
	bool pakGMDoEmote( CPlayer* thisclient, int emotionid );
	bool pakGMCommand( CPlayer* thisclient, CPacket* P );
	bool ReloadMobSpawn( CPlayer* thisclient, int id );
	bool pakGMKick( CPlayer* thisclient, char* name );
	bool pakGMAnn( CPlayer* thisclient, CPacket* P );
	bool pakGMBan( CPlayer* thisclient, char* name );
	bool pakGMInfo( CPlayer* thisclient, char* name );
	bool pakGMNpc( CPlayer* thisclient, int npcid, int dialogid );
	bool pakGMGotomap( CPlayer* thisclient, int map );
	bool pakGMMute( CPlayer* thisclient, char* name, int time );
	bool pakGMPartylvl( CPlayer* partyclient, int level );
	bool pakGMChangePlayerDmg( CPlayer* thisclient, int rate );
	bool pakGMChangeMonsterDmg( CPlayer* thisclient, int rate );
	bool pakGMChangeCfmode( CPlayer* thisclient, int mode );

	//------------------ CHAR SERVER COMMANDS (worldpackets.cpp)
	bool TeleportTo( CPlayer* thisclient, int map, fPoint position );
	bool pakCSCharSelect( CPlayer* thisclient, CPacket* P );
	bool pakCSReady( CPlayer* thisclient, CPacket* P );

	//----------------- Server StartUp Functions (startup.cpp)
	bool LoadSTBData( );
	bool LoadZoneData( );
	bool LoadConsItem( );
	bool LoadSellData( );
	bool LoadProductItem( );
	bool LoadPatItem( );
	bool LoadNaturalItem( );
	bool LoadJemItem( );
	bool LoadEquip( );
	bool LoadItemStats( );
	bool LoadTeleGateData( );
	bool LoadMonsterSpawn( );
#ifdef USEIFO
	bool LoadMobGroups( );
#endif
	bool LoadRespawnData( );
	bool LoadDropsData( );

	bool LoadPYDropsData( );
	bool LoadSkillBookDropsData( );
	bool LoadConfig( );

	bool LoadStatusData( );
	bool LoadSkillData( );
	bool LoadMonsters( );
	bool LoadNPCData( );
	bool LoadUpgrade( );
	bool LoadNPCs( );
	bool LoadSTB( );
	bool CleanConnectedList( );

	void LoadZones( );

	// QSD functions
	void       ReadQSD( std::string path, uint32_t index );
	void       LoadQuestData( );
	inline int round( double x )
	{
		return int( x > 0.0 ? x + 0.5 : x - 0.5 );
	};
	// AIP Functions
	void ReadAIP( std::string path, uint32_t index );
	void LoadAipData( );

	// Server Functions
	bool     SendPM( CPlayer* thisclient, char* Format, ... );
	bool     SendGlobalMSG( CPlayer* thisclient, char* Format, ... );
	uint32_t GetMaxPartyExp( uint32_t partylevel );
	bool     DoSkillScript( CCharacter* character, CSkills* thisskill );

	bool AddParty( CParty* );
	bool RemoveParty( CParty* );

	time_t ClientIDList[ 0x10000 ]; // Clients List
	SOCKET csock;                   // Socket for accessing the char server
	char*  cct;                     // Encryption table for char server

	vector< CTeleGate* > TeleGateList; // Telegates List

	vector< CStatus* > StatusList; // Status List
	vector< CSkills* > SkillList;  // Skills List
	vector< CMDrops* > MDropList;  // Drops List

	vector< CMDrops* > SkillbookList; // Skill book drop list

	vector< CNPCData* >                NPCData;   // NPC/Mobs List
	vector< CParty* >                  PartyList; // Party List
	vector< CFairy* >                  FairyList; // Fairy List
	std::map< unsigned, CNPCTrigger* > NPCActionList;

	CItemStas    StatsList[ 500 ];
	uint32_t     upgrade[ 10 ];
	CEquipList   EquipList[ 10 ];
	CJemList     JemList;
	CNaturalList NaturalList;
	CPatList     PatList;
	CProductList ProductList;
	CSellList    SellList;
	CUseList     UseList;
	CMapList     MapList;

	CSTBData                 STB_NPC;        // NPC data
	CSTBData                 STB_SKILL;      // Skill data
	CSTBData                 STB_STATUS;     // Status Data
	CSTBData                 STB_QUEST;      // Quest data
	CSTBData                 STB_ITEM[ 14 ]; // Item data
	CSTBData                 STB_DROP;       // Drop list
	CSTBData                 STB_ZONE;       // Zone data
	CSTBData                 STB_PRODUCT;    // Crafting data
	CSTBData                 STB_SELL;       // npc sell index
	vector< CQuestTrigger* > TriggerList;
	fpQuestCond              qstCondFunc[ 31 ];
	fpQuestRewd              qstRewdFunc[ 29 ];
	vector< CAip* >          AipList;
	fpAiCond                 aiCondFunc[ 31 ];
	fpAiAct                  aiActFunc[ 38 ];
	//int AIindex[500][6][20][1];

	clock_t     lastServerStep; // Last Update
	clock_t     LastUpdateTime; // Store the last update World Time
	bool        ServerOnline;
	std::string filename;

	std::thread WorldThread[ 3 ];
	std::thread MapThread[ 200 ];
	std::mutex MapMutex, SQLMutex, PlayerMutex;

	CDatabase* DB;
};
extern class CWorldServer* GServer;

PVOID VisibilityProcess( );
PVOID MapProcess( );
PVOID WorldProcess( );
void  ShutdownServer( unsigned int TS );

void StartSignal( );
void StopSignal( );
void HandleSignal( int num );
// -----------------------------------------------------------------------------------------

#endif
