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
#define ClearBattle(i) { i->target=0;i->atktarget=0; i->bufftarget=0; i->skilltarget=0; i->skillid=0; i->atktype=0;  }
typedef unsigned char BYTE;
#include "datatypes.h"

#define TPLAYER 1
#define TMONSTER 2
#define TSUMMON 3

//**************** SHARED ****************//
struct POSITION
{
    fPoint source;          // source position
    fPoint current;         // current position
    fPoint destiny;         // destiny  position
    fPoint skilltargetpos;
    unsigned int Map;       // current map
    unsigned int respawn;   // respawn id [player = respawnid | monster = spawnid]
    unsigned int saved;     // saved id [player town | monster = 0]
    clock_t lastMoveTime;
};

struct STATS
{
    int Level;
    long int HP;
    long int MP;
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
    float Attack_Distance;
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
    bool contatk;
    clock_t lastAtkTime;
    clock_t castTime;
};

struct STATUS
{
    //Stats up
    BYTE Dash_up;
    BYTE Haste_up;
    BYTE Attack_up;
    BYTE Defense_up;
    BYTE Magic_Defense_up;
    BYTE Accury_up;
    BYTE Critical_up;
    BYTE Dodge_up;
    BYTE HP_up;
    BYTE MP_up;
    BYTE MaxHP_up;
    BYTE MaxMP_up;
    BYTE HPMP_up;

    //Stats down
    BYTE Dash_down;
    BYTE Haste_down;
    BYTE Attack_down;
    BYTE Defense_down;
    BYTE Magic_Defense_down;
    BYTE Accury_down;
    BYTE Critical_down;
    BYTE Dodge_down;
    BYTE HP_down;
    BYTE MP_down;
    BYTE MaxHP_down;
    BYTE MaxMP_down;
    BYTE HPMP_down;

    //Status Effects
    BYTE Poisoned;
    BYTE Muted;
    BYTE Sleep;
    BYTE Faint;
    BYTE Stuned;
    BYTE Camo;
    BYTE Invis;
    BYTE Shield;
    BYTE Curse;
    BYTE Recover;
    BYTE Dispell;
    BYTE Detect;
    BYTE Invinc;
    BYTE Flame;
    BYTE ExtraDamage_up;
    BYTE ExtraDamage_down;
    bool CanAttack;
    bool CanRun;
    bool CanMove;
    bool CanCastSkill;

    // Stance
    BYTE Stance;
};

struct MAGICSTATUS
{
    unsigned int Buff;        // Store the buff type
    unsigned int Value;       // Buff Value
    unsigned int Duration;    // Skill Duration
    clock_t BuffTime; // Buff Time
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
    UINT usevalue; // Max Heal Value
    UINT usetype;  // Consumible Type (MP/HP/STAMINA/...)
    UINT userate;  // consumible rate
    UINT used;     // Consumible Used
};

// Player information
struct INFO
{
	char charname[17];
	unsigned int charid;
    BYTE Sex;
    UINT Face;
    UINT Hair;
    UINT Exp;
    UINT Job;
    long int Zulies;
    long int Storage_Zulies;
    long int LastGlobal;
    int StatPoints;
    int SkillPoints;
    unsigned int stamina;
};

struct SESSION
{
	unsigned int userid;
	char username[17];
	char password[33];
	int accesslevel;
	bool isLoggedIn;
	bool inGame;
    bool codedebug;
    int statdebug;
};

struct RIDE
{
    bool Drive;
    bool Ride;
    UINT charid;
};

struct TRADE
{
	unsigned int short trade_itemid[0xa];
	unsigned int trade_count[0xb];
	unsigned int trade_status;
	unsigned int trade_target;
};

struct PARTY
{
	class CParty* party;
	bool IsMaster;
};

struct SHOP
{
    bool open;
    char name[64];
    CBuying BuyingList[30];
    CSelling SellingList[30];
    UINT Buying;
    UINT Selling;
    UINT ShopType;
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
    char clanname[17];
};
struct UNION_s
{
    UINT unionvar[11];
};

//***************Monster only******************************************/

#endif

