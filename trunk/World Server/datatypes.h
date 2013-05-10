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
#ifndef __ROSE_DATATYPES__
#define __ROSE_DATATYPES__

//*upgraded status
//#define NULL 2
#define STRENGTH 10
#define DEXTERY 11
#define INTELIGENCE 12
#define CONCENTRATION 13
#define CHARM 14
#define SENSE 15
#define C_HP 16
#define C_MP 17
#define ATTACK_POWER 18
#define ACCURY 20
#define MAGIC_RESISTENCE 21
#define DODGE 22
#define MOVE_SPEED 23
#define ATTACK_SPEED 24
#define INV_CAPACITY 25
#define CRITICAL 26
#define XP_RATE 30
#define EXTRA_DAMAGE 36
#define ONE_HAND_WEAP_AP 42
#define TWO_HAND_WEAP_AP 43
#define MOV_SPEED 52
#define DEFENSE 53
#define MAX_HP 54
#define MAX_MP 55
#define HP_REC_AMONT 56
#define MP_REC_RATE 57
#define BAGPACK_CAPACITY 58
#define MP_COST_RED 61
#define SUMMON_GAUGE 62
#define ATK_ACCURACY 99
#define SHIELD_DEFENSE 102
#define ONE_HAND_SWORD_AP 104
#define ONE_HAND_MELEE_AP 105
#define TWO_HAND_SWORD_AP 106
#define TWO_HAND_SPEAR_AP 107
#define TWO_HAND_AXE_AP 108
#define BOW_AP 109
#define GUN_AP 110
#define LAUNCHER_AP 111
#define STAFF_POWER 113
#define WAND_POWER 114
#define KATAR_AP 115
#define DUALSWORD_AP 116
#define BOW_ATK_SPD 123
#define GUN_ATK_SPD 124
#define LAUNCHER_ATK_SPD 125
#define KATAR_ATK_SPD 129
#define DUALSWORD_ATK_SPD 130

//*status/condition
#define A_STR 10
#define A_DEX 11
#define A_INT 12
#define A_CON 13
#define A_CHA 14
#define A_SEN 15
#define A_HP 16
#define A_MP 17
#define A_ATTACK 18
#define A_DEFENSE 19
#define A_ACCUR 20
#define A_MRESIST 21
#define A_DODGE 22
#define A_DASH 23
#define A_HASTE 24
#define A_INVENTORY_CAPACITY 25
#define A_CRITICAL 26
#define A_HP_REC_RATE 27
#define A_MP_REC_RATE 28
#define A_MP_CONSUMATION 29
#define A_Extra_Damage 36
#define A_MAX_HP 38
#define A_MAX_MP 39
#define A_GMExtra_Damage 54
#define A_STUN 40
#define A_MUTE 41
#define A_POISON 42


//*spawn values
//BYTE1
#define UNK1 0x01
#define MAX_HP_MP 0x02//revizar
#define POISONED 0x04
#define UNK2 0x08
#define MP_UP 0x10//revizar
#define HP_UP 0x20//revizar
#define DASH_UP 0x40
#define DASH_DOWN 0x80
//BYTE2
#define HASTE_UP 0x01
#define HASTE_DOWN 0x02
#define ATTACK_UP 0x04
#define ATTACK_DOWN 0x08
#define DEFENSE_UP 0x10
#define DEFENSE_DOWN 0x20
#define MDEFENSE_UP 0x40
#define MDEFENSE_DOWN 0x80
//BYTE3
#define HITRATE_UP 0x01
#define HITRATE_DOWN 0x02
#define CRITICAL_UP 0x04
#define CRITICAL_DOWN 0x08
#define DODGE_UP 0x10
#define SUMMON 0x20
#define MUTED 0x40
#define SLEEPING 0x80
//BYTE4
#define STUN 0x01
#define INVISIBLE_1 0x02
#define INVISIBLE_2 0x04
#define UNK3 0x08
#define DAMAGE_UP 0x10
#define SUMMON 0x20
#define UNK4 0x40
#define UNK5 0x80

//Stance
#define mWALKING 0
#define mRUNNING 1
#define ATTACKING 1
#define WALKING 2
#define RUNNING 3
#define DRIVING 4
#define SITTING 5

//Target Type
#define T_MONSTER 1
#define T_PLAYER 2
#define T_SELF 3
#define T_PARTY 4
#define T_OTHER 5

// attack type
#define NORMAL_ATTACK 1
#define SKILL_ATTACK 2
#define SKILL_BUFF 3
#define SKILL_AOE 4
#define SKILL_SELF 5
#define BUFF_SELF 6
#define BUFF_AOE 7
#define AOE_TARGET 8

// other constants
#define MAX_SKILL 90
#define SKILL_DELAY 500 //ms

// weapon type
#define ONE_HAND_SWORD 211
#define ONE_HAND_BUNT_SWORD 212
#define TWO_HAND_SWORD 221
#define SPEAR 222
#define TWO_HAND_AXE 223
#define BOW 231
#define GUN 232
#define LAUNCHER 233
#define STAFF 241
#define WAND 242
#define KATAR 251
#define DOUBLE_SWORD 252
#define DUAL_GUN 253
#define CROSSBOW 271

//equip type
#define MASK 1
#define CAP 2
#define BODY 3
#define GLOVE 4
#define SHOE 5
#define BACK 6
#define JEWEL 7
#define WEAPON 8
#define SUBWEAPON 9
#define CONSUMIBLE 10
#define JEM 11
#define NATURAL 12
#define QUEST 13
#define PAT 14

//time
#define DAY 0
#define MORNING 1
#define EVENING 2
#define NIGHT 3

// Quests

#define sIsSummon 1
#define sGender 2
#define sBirthStone 3
#define sJob 4
#define sUnion 5
#define sUnionPoints 83
#define sRanking 6
#define sReputation 7
#define sFace 8
#define sHair 9

#define sStrength 10
#define sDexterity 11
#define sIntelligence 12
#define sConcentration 13
#define sCharm 14
#define sSensibility 15

#define sHP 16
#define sMP 17

#define sAttackPower 18
#define sDefense 19
#define sAccuracy 20
#define sMagicResistance 21
#define sDodge 22
#define sMoveSpeed 23
#define sAttackSpeed 24
#define sMaxWeight 25
#define sCritical 26

#define sHPRate 27
#define sMPRate 28
#define sMPDecrease 29

#define sEXPRate 30
#define sLevel 31
#define sStatPoints 32
#define sTendency 33
#define sPvp 34
#define sHeadSize 35
#define sBodySize 36
#define sSkillPoints 37

#define sMaxHP 38
#define sMaxMP 39

#define sMoney 40
#define sUnarmedAtkPow 41
#define sOneHandAtkPow 42
#define sTwoHandAtkPow 43
#define	sBowAtkPow 44
#define sGunAtkPow 45
#define sMagicAtkPow 46
#define sCrossbowAtkPow 47
#define sCombatAtkPow 48

#define sBowAtkSpd 49
#define sGunAtkSpd 50
#define sCombatAtkSpd 51

#define sMoveSpeed2 52	 //Passive
#define sDefense2 53	 //Passive

#define sMaxHP2 54		 //Passive
#define sMaxMP2 55		 //Passive
#define sHPRecovery 56
#define sMPRecovery 57

#define sMaxWeight2 58
#define sBuyDiscount 59
#define sSalesPremium 60
#define sMPSpendDecrease 61
#define sExpandSummonGauge 62
#define sItemDropIncrease 63
#define sIncreaseSummonGauge 63

#define sCurrentPlanet 75
#define sStamina 76

#define sClanPoints 92

#define sNoStorageCharge 93
#define sExpandStorage 95
#define sRemodelShop 96
#define sCartGauge 97

// Stat Type
#define sNone 0
#define sPoison 7
#define sPoison2 8
#define sPoison3 9
#define sPoison4 10
#define sPoison5 11
#define sMaxHPUp 12
#define sMaxMPUp 13
#define sDash 14
#define sSlow 15
#define sHasteAtk 16
#define sSlowAtk 17
#define sAtkUp 18
#define sAtkDown 19
#define sDefUp 20
#define sDefDown 21
#define sMResUp 22
#define sMRresDown 23
#define sAccUp 24
#define sAccDown 25
#define sCritUp 26
#define sCritDown 27
#define sDodgeUp 28
#define sDodgeDown 29
#define sMute 30
#define sSleep 31
#define sFaint 32
#define sCamo 33
#define sInvis 34
#define sShield 35
#define sAddDmg 36
#define sCurse 37
#define sRecover 38
#define sDispell 39
#define sHPMPRest 40
#define sHPMPRest2 41
#define sHPMPRest3 42
#define sHPDown 43
#define sMaxHPUp2 44
#define sMaxMPUp2 45
#define sDash2 46
#define sHasteAtk2 47
#define sAtkUp2 48
#define sDefUp2 49
#define sMResUp2 50
#define sAccUp2 51
#define sCritUp2 52
#define sDodgeUp2 53
#define sAddDmg2 54
#define sDetect 55
#define sTaunt 56
#define sInvinc 57
#define sFlame 58
#define sDefDown2 59
#define sSlow2 60
#define sFlame2 61
#define sHPRes100 62
#define sHPRes200 63
#define sHPRes300 64
#define sHPRes40  65
#define sHPRes80  66
#define sHPRes120 67
#define sMPRes40  68
#define sMPRes80  69
#define sMPRes120 70
#define sMPRes10  71
#define sMPRes15  72
#define sMPRes20  73
#define sFlame3 74
#define sHPRes160 75
#define sMPRes25  76
#define sFlame4 77
#define sFlame5 78
#define sFlame6 79
#define sFlame7 80


// Skills - Classes
#define cVisitor 0
#define cSoldier 1
#define cMuse 2
#define cHawker 3
#define cDealer 4
#define cKnight 11
#define cChampion 12
#define cMage 13
#define cCleric 14
#define cRaider 15
#define cScout 16
#define cBourgeois 17
#define cArtisan 18
#define cSoldierJob 41
#define cMuseJob 42
#define cHawkerJob 43
#define cDealerJob 44
#define cFirstJob 46
#define cSecondJob 47
#define cThirdJob 48
#define cSoldier2nd 51
#define cMuse2nd 52
#define cHawker2nd 53
#define cDealer2nd 54
#define cSoldier3rd 56
#define cMuse3rd 57
#define cHawker3rd 58
#define cDealer3rd 59
#define cKnightJob 61
#define cChampionJob 62
#define cMageJob 63
#define cClericJob 64
#define cRaiderJob 65
#define cScoutJob 66
#define cBourgeoisJob 67
#define cArtisanJob 68
#define cSoldierHawker 71

// Skills - Types
#define tBasic 1
#define tCraft 2
#define tDamage 3           // Seen (Start)
#define tWeaponUp 4
#define tWeaponUp2 5
#define tSpell 6            // Seen (Start) (This one is odd, an AOE skill, that uses Start)
#define tAOEDamage 7        // Seen (AOE)
#define tContinueSelf2 8    // Seen (Self)
#define tContinuing2  9
#define tRecoverSelf 10     // Seen (Self)
#define tRecover 11         // Seen (Start)
#define tContinueSelf 12    // Seen (Self)
#define tContinue 13
#define tSummon 14
#define tPassive 15
#define tEmotion 16
#define tAOEAttackSelf 17   // Seen (Self)
#define tReservation 18
#define tAbsorb 19
#define tResurrect 20

// Skills - Targets
#define tYourself 0
#define tPartyMember 1
#define tClanMember 2
#define tAlly 3
#define tMonster 4
#define tHostileCharacter 5
#define tHostileTeamMember 6
#define tAllCharacters 7
#define tAllMembers 8
#define tFaintedAlly 9
#define tHostileMonster 10

// Stats calc - Jobs
#define jVisitor 0
#define jSoldier 111
#define jKnight 121
#define jChampion 122
#define jMuse 211
#define jMage 221
#define jCleric 222
#define jHawker 311
#define jRaider 321
#define jScout 322
#define jDealer 411
#define jBourgeois 421
#define jArtisan 422

// HitPower Bits
#define UNK1BIT 0x2000
#define CRITBIT 0x4000
#define KILLBIT 0x8000



#include "../common/sockets.h"

// Hold party experience when kill a moster
struct CPartyExp
{
    class CParty* thisparty;
    UINT exp;
    UINT num;
    UINT partymember[50];
    UINT maxlevel;
    bool flag;
};

// Equip information
struct CEquip
{
    UINT id;
    UINT itemtype;
    UINT equiptype;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT level;
    UINT material;
    UINT occupation[4];
    UINT condition1[3];
    UINT condition2[3];
    UINT stat1[3];
    UINT stat2[3];
    UINT defense;
    UINT magicresistence;
    UINT attackdistance;
    UINT attackpower;
    UINT attackspeed;
    UINT magicattack;
    UINT itemgrade;
    UINT movespeed;
    UINT raretype;
    UINT unionpoint;
};

// Jem Data
struct CJemData
{
    UINT id;
    UINT itemtype;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT material;
    UINT stat1[2];/**/
    UINT stat2[2];/**/
};

// natural data
struct CNaturalData
{
    UINT id;
    UINT itemtype;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT pricevalue;
};

// pat data
struct CPatData
{
    UINT id;
    UINT itemtype;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT material;
    UINT partversion;
    UINT level;
    UINT condition[2];
    UINT modifier[2];
    UINT maxfuel;
    UINT fuelcons;
    UINT speed;
    UINT attackdistance;
    UINT attackpower;
    UINT attackspeed;
};

struct CProductData
{
    UINT id;
    UINT item[50];
    UINT amount[50];
};

struct CCSellData
{
    UINT id;
    UINT item[49];
};

struct CUseData
{
    UINT id;;
    UINT restriction;
    UINT type;
    UINT price;
    UINT pricerate;
    UINT weight;
    UINT quality;
    UINT usecondition[2];
    UINT useeffect[2];
    UINT pricevalue;
};

struct CVector2D
{
	CVector2D() : x(0), y(0) {}
	CVector2D(dword nX, dword nY) : x(nX), y(nY) {}
	dword x, y;

	bool operator == ( const CVector2D& otherpoint ){
		return (x == otherpoint.x) && (y == otherpoint.y);
	}

	bool operator != ( const CVector2D& otherpoint ){
		return (x != otherpoint.x) || (y != otherpoint.y);
	}
};

struct CVector2F
{
	CVector2F() : x(0), y(0) {}
	CVector2F(float nX, float nY) : x(nX), y(nY) {}
	float x, y;

	bool operator == ( const CVector2F& otherpoint )
	{
		return (fabs(this->x-otherpoint.x)<0.001f && fabs(this->y-otherpoint.y)<0.001f);
	}
};

struct SBasic
{
	word exp;
	word map;
	CVector2F pos;
};

// -----------------------------------------------------------------------------------------
// A 2d point, for positions
// -----------------------------------------------------------------------------------------
struct fPoint {
	float x;
	float y;
	float z; // Unused?
};

// List Mob Data
struct CNPCData {
    UINT id;
    UINT life;
    UINT wspeed;
    UINT rspeed;
    BYTE stance;
    UINT dspeed;
    UINT weapon;
    UINT subweapon;
    UINT level;
    UINT hp;
    UINT atkpower;
    UINT hitrate;
    UINT defense;
    UINT magicdefense;
    UINT dodge;
    float atkspeed;
    UINT magicattack;
    UINT AI;
    UINT exp;
  	UINT dropid;
  	UINT money;
  	UINT item;
  	UINT tab1;
  	UINT tab2;
  	UINT tab3;
  	UINT specialtab;
    float atkdistance;
    UINT aggresive;
    UINT shp;
    UINT dialogid;

    UINT side;
    UINT sidechance;
    UINT zulydrop;
    UINT mapdrop;
    UINT mobdrop;
    UINT leveldrop;
    UINT dropchance;

    dword EventID;
    SBasic basic;
	bool IsMoving;
	CVector2F newPos;
	CVector2D lastBlock;
	CVector2D curBlock;
	dword _EntityType;
};

// Store the damage for monster to give exp
struct MonsterDamage
{
    UINT charid;
    long int damage;
};

struct CDropInfo
{
    BYTE type;
    DWORD item;
    QWORD prob;
};

// For store the drops info
struct CMDrops
{
    UINT id;
    UINT zuly;
    UINT zulyprob;
    vector<CDropInfo*> Drops;
    UINT level_min;//for map drops
    UINT level_max;//for map drops
    UINT level_boss;//for map drops
    UINT probmax;

    UINT prob;
    UINT map;
    UINT mob;
    UINT itemtype;
    UINT itemnum;
    UINT alt[8];

};

// For store the Buff info
struct CBuff
{
    unsigned int Buff;//store the buff type
    unsigned int value1;//buff value 1 (direct)
    unsigned int value2;//buff value 2 (%)
    unsigned int Evalue;//Extra value 1 (buffer int +)
    unsigned int Duration; //Skill Duration
    clock_t bufftime;//Buff Time
};

// -----------------------------------------------------------------------------------------
// For store the Buff info
// -----------------------------------------------------------------------------------------
struct CBuffValue
{
    int value1;
    int value2;
    int value3;
};

// -----------------------------------------------------------------------------------------
// For store the Buff info
// -----------------------------------------------------------------------------------------
struct CPSkill
{
    int buff[3];//Buff type
    int value1[3];
    int value2[3];
};

// -----------------------------------------------------------------------------------------
// For store the clan info
// -----------------------------------------------------------------------------------------
struct SClan
{
    int id;
    int logo;
    int back;
    int grade;
    int cp;
    char name[50];
    char slogan[100];
    char news[200];
};

// -----------------------------------------------------------------------------------------
// GM Respawn Mobs  (50 points max)
// -----------------------------------------------------------------------------------------
struct CRespawnPoints
{
    int d;//flag to duplicate
    int b;//flag to store
    int n;//num points
    int min;//mob min
    int max;//mob max
    int map;//map
    int mobID;//mob id
    int respawntime;//respawn time
    fPoint points[50];//points
};

// -----------------------------------------------------------------------------------------
// An item that a client owns
// -----------------------------------------------------------------------------------------
class CItem
{
public:
    unsigned short GetPakHeader( );
	unsigned GetPakData( );
	unsigned GetVisInfo( );
	void Clear( );

	inline bool IsStackable()
	{
		return (itemtype >= 10) && (itemtype <= 13);
	};
	unsigned		itemnum;
	UINT	itemtype;
	UINT	refine;
	UINT	lifespan;
	UINT	durability;
	bool			socketed;
	bool			appraised;
	int				count;
	UINT 	stats;
	UINT    gem;
	UINT    quality;
	UINT    type;
};

// -----------------------------------------------------------------------------------------
// Selling / buying items
// -----------------------------------------------------------------------------------------
struct BSItem {
    int slot;
    int count;
    long int price;
    int head;
    int data;
};

// -----------------------------------------------------------------------------------------
// A typical npc
// -----------------------------------------------------------------------------------------
struct CNPC {
	unsigned short clientid;
	unsigned char posMap;
	unsigned npctype;
	fPoint pos;
	float dir;
	CNPCData* thisnpc;
};

struct SWarp {
	SWarp() : TargetPoint(0), TargetMap(0), SourceMap(0){pos.x = 0; pos.y = 0;}
	~SWarp(){}

	CVector2F pos;
	dword SourceMap;
	dword TargetMap;
	strings TargetPoint;
};

// Zone Data
struct SZonPoint {
	CVector2F pos;
	strings name;
};

// Item data object
struct CItemData
{
	unsigned type;
	unsigned id;
};

// Structure for holding loaded STB data

struct CNPCTrigger
{
	unsigned hash;
	unsigned char linkcount;
	unsigned char conditioncount;
	unsigned char rewardcount;
	unsigned* links;
	unsigned char** actions;
};

// -----------------------------------------------------------------------------------------
// Skill Data
// -----------------------------------------------------------------------------------------
struct CSkills {
    UINT id;
    UINT level;
    UINT type;
    UINT skilltype; // New
    UINT range;
    UINT target;
    UINT status[2]; // New
    UINT aoerange; // New
    UINT successrate; // New
    UINT costtype[2]; // New
    UINT costamount[2]; // New
    UINT duration;
    UINT atkpower;
    UINT mp;
    UINT costtype2; // New
    UINT costamount2; // New
    UINT cooldown; // New
    UINT weapon[5];
    UINT rskill[3];
    UINT lskill[3];
    UINT c_class[4];
    UINT req[2]; // New
    UINT reqam[2]; // New
    UINT zuly;   // New
    UINT clevel;
    UINT success;
    UINT sp;
    UINT buff[3];
    UINT value1[3];
    UINT value2[3];
    UINT nbuffs;
    unsigned short int aoe;
    UINT aoeradius;
    UINT script;
    UINT svalue1;
};

// -----------------------------------------------------------------------------------------
// Status Object
// -----------------------------------------------------------------------------------------
struct CStatus
{
  UINT id; //
  UINT type; // Unk [1]
  UINT duplication; // Unk [2]
  UINT dir; // 0: Up 1: Down 2: ? [3]
  UINT repeat; // 1: Repeat 2: Once 3: Special [4]
  UINT ability[2]; // Status # to call? [5][7]
  UINT amount[2]; // # to increase/decrease by? [6][8]
  UINT decrease; // 1: decrease 0: increase [17]
};


// -----------------------------------------------------------------------------------------
// Sell data object
// -----------------------------------------------------------------------------------------
struct CSellData
{
	unsigned id;
	CItem items[48];
};

// -----------------------------------------------------------------------------------------
// User skill info
// -----------------------------------------------------------------------------------------
struct CSkill
{
	unsigned id;
	unsigned level;
	CSkills* thisskill;
};

// -----------------------------------------------------------------------------------------
// An item or zuly dropped on the ground
// -----------------------------------------------------------------------------------------
struct CDrop
{
	unsigned short clientid;
	unsigned char posMap;
	fPoint pos;
	char type;
	unsigned amount;
	CItem item;
	time_t droptime;
	unsigned short owner;
    class CParty* thisparty;
};

// -----------------------------------------------------------------------------------------
// pvp zones
// -----------------------------------------------------------------------------------------
struct CPVPZones
{
    int map;
};
// -----------------------------------------------------------------------------------------
// A teleport gate object
// -----------------------------------------------------------------------------------------
struct CTeleGate
{
	unsigned short id;
	fPoint src;
	unsigned char srcMap;
	fPoint dest;
	unsigned char destMap;
};

// -----------------------------------------------------------------------------------------
// A respawn point object
// -----------------------------------------------------------------------------------------
struct CRespawnPoint
{
	unsigned short id;
	fPoint dest;
	unsigned char destMap;
	unsigned char radius;
	bool masterdest;
};

// -----------------------------------------------------------------------------------------
// A monster spawn zone
// -----------------------------------------------------------------------------------------
struct CSpawnArea
{
	UINT id;
	UINT map;
	UINT min;
	UINT max;
	UINT respawntime;
	UINT montype;
	UINT pcount;
	UINT amon;
	fPoint* points;
	clock_t lastRespawnTime;
	CNPCData* thisnpc;
	CMDrops* mobdrop;
	CMDrops* mapdrop;
};

#ifdef USEIFO
struct CMob {
  UINT id;
  UINT mobId;
  bool tactical;
  UINT amount;
  CNPCData* thisnpc;
	CMDrops* mobdrop;
	CMDrops* mapdrop;
};

struct CMobGroup {
  UINT id;
  UINT map;
  UINT limit;
  UINT active;
  UINT tacticalpoints;
  UINT respawntime;
  UINT basicKills;
  UINT curTac;
  UINT curBasic;
  fPoint point;
  UINT range;
  clock_t lastRespawnTime;
  vector<CMob*> basicMobs;
  vector<CMob*> tacMobs;
};
#endif

struct CUseInfo
{
    int itemnum;
    int itemtype;
    int usescript;
    int usetype;
    int usevalue;
};

struct CItemType
{
    int itemnum;
    int itemtype;
    int probability;
};

struct CItemSlot
{
    UINT newslot[2];
    UINT nslot;
    bool flag;
};

// Buying List (Shop)
struct CBuying
{
    CItem item;
    long int price;
    UINT slot;
    UINT count;
};

// Selling List (Shop)
struct CSelling
{
    UINT slot;
    UINT count;
    long int price;
};

// MONSTER <---------------------------
struct MPosition // Monster position
{
    UINT Map;
    UINT SpawnID;
    fPoint Spos; // Start position
    fPoint Cpos; // Current Position
    fPoint Dpos; // destiny  Position
};

struct MDrop    // monster drops
{
    bool drop;
    unsigned int firsthit;
    unsigned int firstlevel;
	CMDrops* mobdrop;
	CMDrops* mapdrop;
};

struct CBValue
{
    UINT NewValue;
    UINT Value;
    UINT Position;
};


// ITEMS
struct CEquipList
{
    CEquip *Index[5000];
    vector<CEquip*> Data;
    CEquip* nullequip;
};

struct CJemList
{
    CJemData *Index[4000];
    vector<CJemData*> Data;
    CJemData* nulljem;
};

struct CNaturalList
{
    CNaturalData *Index[1000];
    vector<CNaturalData*> Data;
    CNaturalData* nullnatural;
};


struct CPatList
{
    CPatData *Index[1000];
    vector<CPatData*> Data;
    CPatData* nullpat;
};

struct CProductList
{
    CProductData* Index[2000];
    vector<CProductData*> Data;
    CProductData* nullproduct;
};

struct CSellList
{
    CCSellData* Index[1000];
    vector<CCSellData*> Data;
    CCSellData* nullsell;
};

struct CUseList
{
    CUseData* Index[2000];
    vector<CUseData*> Data;
    CUseData* nulluse;
};

struct CMapList
{
    class CMap* Index[300];
    vector<class CMap*> Map;
    class CMap* nullzone;
};

struct CItemStas
{
  UINT stat[2];
  UINT value[2];
};

struct CFairy
{
    UINT ListIndex;
	clock_t LastTime;
    UINT WaitTime;
    bool assigned;
};

// Quests

struct SQuest
{
	word QuestID;
	dword StartTime;
	word Variables[10];
	byte Switches[4];
	CItem Items[5];
	byte unknown[6];

	bool GetSwitchBit( dword switchId ){
		dword byteId = switchId / 8;
		dword bitId = switchId % 8;
		return ((Switches[byteId] >> bitId) & 1);
	}

	void SetSwitchBit( byte switchId, byte value ){
		dword byteId = switchId / 8;
		dword bitId = switchId % 8;
		Switches[byteId] = Switches[byteId] | ((value?1:0) << bitId);
	}

	void AddItem(CItem* item, byte btOp = 2){
		for(dword i = 0; i < 6; i++){
			if(Items[i].GetPakHeader() == item->GetPakHeader()) {
				if(btOp == 1){
					Items[i].count += item->count;
					return;
				}else if(btOp == 0){
					if(Items[i].count <= item->count)
						Items[i].Clear();
					else
						Items[i].count -= item->count;
					return;
				}
			}
			if(btOp == 0) continue;
			if(Items[i].GetPakHeader() != 0) continue;
			Items[i].itemnum = item->itemnum;
			Items[i].itemtype = item->itemtype;
			Items[i].count = item->count;
			return;
		}
	}

	void Clear(){
		memset(this, 0, sizeof(SQuest));
	}

};

struct SQuestData
{
	word EpisodeVar[5];
	word JobVar[3];
	word PlanetVar[7];
	word UnionVar[10];

	SQuest quests[10];
	byte flags[0x40];

	void SetFlag( dword flagid, bool value ){
		dword byteid = flagid / 8;
		dword bitid = flagid % 8;
		flags[byteid] = flags[byteid] | ((value?1:0) << bitid);
	}

	bool GetFlag( dword flagid ){
		dword byteid = flagid / 8;
		dword bitid = flagid % 8;
		return ((flags[byteid] >> bitid) & 1);
	}
};

#endif

