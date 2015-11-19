// Props to ExJam for this code :D Ported to OSpRose by Drakia
#define QUESTREWD( reward ) int QUEST_REWD_##reward( class CWorldServer* server, class CPlayer* client, uint8_t* raw )
typedef int ( *fpQuestRewd )( class CWorldServer*, class CPlayer*, uint8_t* );
#define GETREWDDATA( rewd ) STR_REWD_##rewd* data = (STR_REWD_##rewd*)raw

QUESTREWD( 000 );
QUESTREWD( 001 );
QUESTREWD( 002 );
QUESTREWD( 003 );
QUESTREWD( 004 );
QUESTREWD( 005 );
QUESTREWD( 006 );
QUESTREWD( 007 );
QUESTREWD( 008 );
QUESTREWD( 009 );
QUESTREWD( 010 );
QUESTREWD( 011 );
QUESTREWD( 012 );
QUESTREWD( 013 );
QUESTREWD( 014 );
QUESTREWD( 015 );
QUESTREWD( 016 );
QUESTREWD( 017 );
QUESTREWD( 018 );
QUESTREWD( 019 );
QUESTREWD( 020 );
QUESTREWD( 021 );
QUESTREWD( 022 );
QUESTREWD( 023 );
QUESTREWD( 024 );
QUESTREWD( 025 );
QUESTREWD( 026 );
QUESTREWD( 027 );
QUESTREWD( 028 );

struct STR_REWD_000
{
	uint32_t iQuestSN;
	uint8_t  btOp;
};

struct STR_REWD_001
{
	uint32_t uiItemSN;
	union
	{
		uint8_t  btOp;
		uint16_t nOp;
	};
	uint16_t nDupCNT;
	uint8_t  btPartyOpt;
};

struct STR_REWD_002
{
	uint32_t        iDataCnt;
	STR_QUEST_DATA* CheckData;
};

struct STR_REWD_003
{
	uint32_t       iDataCnt;
	STR_ABIL_DATA* CheckData;
	uint8_t        btPartyOpt;
};

struct STR_REWD_004
{
	uint32_t        iDataCnt;
	STR_QUEST_DATA* CheckData;
};

struct STR_REWD_005
{
	uint8_t btTarget;
	union
	{
		uint8_t  btEquation;
		uint16_t nEquation;
	};
	int      iValue;
	uint32_t iItemSN;
	union
	{
		uint8_t  btPartyOpt;
		uint16_t nPartyOpt;
	};
	uint16_t nItemOpt;
};

struct STR_REWD_006
{
	uint32_t iPercentOfHP;
	uint32_t iPercentOfMP;
	uint8_t  btPartyOpt;
};

struct STR_REWD_007
{
	uint32_t iZoneSN;
	uint32_t iX;
	uint32_t iY;
	uint8_t  btPartyOpt;
};

struct STR_REWD_008
{
	uint32_t iMonsterSN;
	uint32_t iHowMany;
	union
	{
		uint8_t btWho;
		int     iWho;
	};
	uint32_t iZoneSN;
	uint32_t iX;
	uint32_t iY;
	uint32_t iRange;
	uint32_t iTeamNo;
};

struct STR_REWD_009
{
	uint16_t shNameLen;
	string   szNextTriggerSN;
};

struct STR_REWD_010
{
};

struct STR_REWD_011
{
	union
	{
		uint8_t  btWho;
		uint16_t usWho;
	};
	uint16_t nVarNo;
	int      iValue;
	uint8_t  btOp;
};

struct STR_REWD_012
{
	union
	{
		uint8_t btMsgType;
		int     iMsgType;
	};
	uint32_t iStrID;
};

struct STR_REWD_013
{
	union
	{
		uint8_t btWho;
		int     iWho;
	};
	uint32_t iSec;
	uint16_t nNameLength;
	string   szTriggerName;
	uint32_t m_HashNextTrigger;
};

struct STR_REWD_014
{
	union
	{
		uint8_t btOp;
		int     iOp;
	};
	uint32_t iSkillNo;
};

struct STR_REWD_015
{
	uint16_t nSN;
	uint8_t  btOp;
};

struct STR_REWD_016
{
	uint16_t nGroupSN;
};

struct STR_REWD_017
{
};

struct STR_REWD_018
{
	uint32_t iStrID;
	uint16_t nCnt;
	string   Data;
};

struct STR_REWD_019
{
	uint16_t nZoneNo;
	uint16_t nTeamNo;
	uint16_t nTriggerLength;
	string   TriggerName;
	uint32_t m_HashTrigger;
};

struct STR_REWD_020
{
	uint8_t btNoType;
};

struct STR_REWD_021
{
	uint32_t iX;
	uint32_t iY;
};

struct STR_REWD_022
{
	uint16_t nZonNo;
	uint8_t  btOp;
};

struct STR_REWD_023
{
};

struct STR_REWD_024
{
	int     iMONEY;
	uint8_t btOP;
};

struct STR_REWD_025
{
	short   nPOINT;
	uint8_t btOP;
};

struct STR_REWD_026
{
	uint16_t nSkillNo;
	uint8_t  btOP;
};

struct STR_REWD_027
{
	short   nCONT;
	uint8_t btOP;
};

struct STR_REWD_028
{
	uint32_t iRange;
	uint16_t nZoneNo;
	uint32_t iX;
	uint32_t iY;
};
