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

    depeloped with Main erose/hrose source server + some change from the original eich source
*/
#ifndef _CHARACTER_TYPE_
#define _CHARACTER_TYPE_
#define MAX_STORAGE 160
#define MAX_INVENTORY 140
#define MAX_BASICSKILL 42
#define MAX_QUICKBAR 32
#define ClearBattle( i )    \
	{                       \
		i->target      = 0; \
		i->atktarget   = 0; \
		i->bufftarget  = 0; \
		i->skilltarget = 0; \
		i->skillid     = 0; \
		i->atktype     = 0; \
	}

#include "datatypes.h"

#define TPLAYER 1
#define TMONSTER 2
#define TSUMMON 3

//**************** SHARED ****************//
struct POSITION
{
	fPoint       source;  // source position
	fPoint       current; // current position
	fPoint       destiny; // destiny  position
	fPoint       skilltargetpos;
	unsigned int Map;     // current map
	unsigned int respawn; // respawn id [player = respawnid | monster = spawnid]
	unsigned int saved;   // saved id [player town | monster = 0]
	clock_t      lastMoveTime;
};

struct STATS
{
	int          Level;
	long int     HP;
	long int     MP;
	unsigned int MaxHP;
	unsigned int MaxMP;

	unsigned int Attack_Power;
	unsigned int Defense;
	unsigned int Critical;
	unsigned int Dodge;
	unsigned int Accury;
	unsigned int Magic_Defense;
	unsigned int Move_Speed;
	unsigned int Mspd_base;
	unsigned int Attack_Speed;
	float        Attack_Distance;
	unsigned int MaxWeight;
	unsigned int MaxSummonGauge;
	unsigned int MPReduction;
	unsigned int ExtraDamage;
	unsigned int xprate;
	unsigned int magicattack;
};

struct BATTLE
{
	unsigned int target;
	unsigned int atktarget;
	unsigned int bufftarget;
	unsigned int skilltarget;
	unsigned int skillid;
	unsigned int atktype;
	bool         contatk;
	clock_t      lastAtkTime;
	clock_t      castTime;
};

struct STATUS
{
	//Stats up
	uint8_t Dash_up;
	uint8_t Haste_up;
	uint8_t Attack_up;
	uint8_t Defense_up;
	uint8_t Magic_Defense_up;
	uint8_t Accury_up;
	uint8_t Critical_up;
	uint8_t Dodge_up;
	uint8_t HP_up;
	uint8_t MP_up;
	uint8_t MaxHP_up;
	uint8_t MaxMP_up;
	uint8_t HPMP_up;

	//Stats down
	uint8_t Dash_down;
	uint8_t Haste_down;
	uint8_t Attack_down;
	uint8_t Defense_down;
	uint8_t Magic_Defense_down;
	uint8_t Accury_down;
	uint8_t Critical_down;
	uint8_t Dodge_down;
	uint8_t HP_down;
	uint8_t MP_down;
	uint8_t MaxHP_down;
	uint8_t MaxMP_down;
	uint8_t HPMP_down;

	//Status Effects
	uint8_t Poisoned;
	uint8_t Muted;
	uint8_t Sleep;
	uint8_t Faint;
	uint8_t Stuned;
	uint8_t Camo;
	uint8_t Invis;
	uint8_t Shield;
	uint8_t Curse;
	uint8_t Recover;
	uint8_t Dispell;
	uint8_t Detect;
	uint8_t Invinc;
	uint8_t Flame;
	uint8_t ExtraDamage_up;
	uint8_t ExtraDamage_down;
	bool CanAttack;
	bool CanRun;
	bool CanMove;
	bool CanCastSkill;

	// Stance
	uint8_t Stance;
};

struct MAGICSTATUS
{
	unsigned int Buff;     // Store the buff type
	unsigned int Value;    // Buff Value
	unsigned int Duration; // Skill Duration
	clock_t      BuffTime; // Buff Time
};

//**************************************************************/
//***************Player only******************************************/
struct ATTRIBUTES
{
	unsigned int Str;
	unsigned int Dex;
	unsigned int Int;
	unsigned int Con;
	unsigned int Cha;
	unsigned int Sen;

	unsigned int Estr;
	unsigned int Edex;
	unsigned int Eint;
	unsigned int Econ;
	unsigned int Echa;
	unsigned int Esen;
};

struct USEDITEM
{
	clock_t lastRegTime; // Last Regeneration
	uint32_t    usevalue;    // Max Heal Value
	uint32_t    usetype;     // Consumible Type (MP/HP/STAMINA/...)
	uint32_t    userate;     // consumible rate
	uint32_t    used;        // Consumible Used
};

// Player information
struct INFO
{
	char         charname[ 17 ];
	unsigned int charid;
	uint8_t         Sex;
	uint32_t         Face;
	uint32_t         Hair;
	uint32_t         Exp;
	uint32_t         Job;
	long int     Zulies;
	long int     Storage_Zulies;
	long int     LastGlobal;
	int          StatPoints;
	int          SkillPoints;
	unsigned int stamina;
};

struct SESSION
{
	unsigned int userid;
	char         username[ 17 ];
	char         password[ 33 ];
	int          accesslevel;
	bool         isLoggedIn;
	bool         inGame;
	bool         codedebug;
	int          statdebug;
};

struct RIDE
{
	bool Drive;
	bool Ride;
	uint32_t charid;
};

struct TRADE
{
	unsigned int short trade_itemid[ 0xa ];
	unsigned int       trade_count[ 0xb ];
	unsigned int       trade_status;
	unsigned int       trade_target;
};

struct PARTY
{
	class CParty* party;
	bool          IsMaster;
};

struct SHOP
{
	bool     open;
	char     name[ 64 ];
	CBuying  BuyingList[ 30 ];
	CSelling SellingList[ 30 ];
	uint32_t     Buying;
	uint32_t     Selling;
	uint32_t     ShopType;
};

struct SKILLS
{
	unsigned id;
	unsigned level;
	CSkills* thisskill;
};

struct CLAN
{
	unsigned int clanid;
	unsigned int clanrank;
	unsigned int grade;
	unsigned int CP;
	unsigned int logo;
	unsigned int back;
	char         clanname[ 17 ];
};
struct UNION_s
{
	uint32_t unionvar[ 11 ];
};

//***************Monster only******************************************/

#endif
