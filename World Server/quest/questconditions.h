// Props to ExJam for this code :D Ported to OSpRose by Drakia
#define QUESTCOND( cond ) int QUEST_COND_##cond( class CWorldServer* server, class CPlayer* client, uint8_t* raw )
typedef int ( *fpQuestCond )( class CWorldServer*, class CPlayer*, uint8_t* );
#define GETCONDDATA( cond ) STR_COND_##cond* data = (STR_COND_##cond*)raw

QUESTCOND( 000 );
QUESTCOND( 001 );
QUESTCOND( 002 );
QUESTCOND( 003 );
QUESTCOND( 004 );
QUESTCOND( 005 );
QUESTCOND( 006 );
QUESTCOND( 007 );
QUESTCOND( 008 );
QUESTCOND( 009 );
QUESTCOND( 010 );
QUESTCOND( 011 );
QUESTCOND( 012 );
QUESTCOND( 013 );
QUESTCOND( 014 );
QUESTCOND( 015 );
QUESTCOND( 016 );
QUESTCOND( 017 );
QUESTCOND( 018 );
QUESTCOND( 019 );
QUESTCOND( 020 );
QUESTCOND( 021 );
QUESTCOND( 022 );
QUESTCOND( 023 );
QUESTCOND( 024 );
QUESTCOND( 025 );
QUESTCOND( 026 );
QUESTCOND( 027 );
QUESTCOND( 028 );
QUESTCOND( 029 );
QUESTCOND( 030 );

struct STR_COND_000
{
	int iQuestSN;
};

struct STR_COND_001
{
	int             iDataCnt;
	STR_QUEST_DATA* CheckData;
};

struct STR_COND_002
{
	int             iDataCnt;
	STR_QUEST_DATA* CheckData;
};

struct STR_COND_003
{
	int            iDataCnt;
	STR_ABIL_DATA* CheckData;
};

struct STR_COND_004
{
	int            iDataCnt;
	STR_ITEM_DATA* CheckData;
};

struct STR_COND_005
{
	uint8_t btIsLeader;
	int     iLevel;
	uint8_t btReversed;
};

struct STR_COND_006
{
	int iZoneSN;
	int iX;
	int iY;
	int iZ;
	int iRadius;
};

struct STR_COND_007
{
	uint32_t ulTime;
	uint32_t ulEndTime;
};

struct STR_COND_008
{
	uint32_t ulTime;
	uint8_t  btOp;
};

struct STR_COND_009
{
	int     iSkillSN1;
	int     iSkillSN2;
	uint8_t btOp;
};

struct STR_COND_010
{
	uint8_t btLowPcnt;
	uint8_t btHighPcnt;
};

struct STR_COND_011
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

struct STR_COND_012
{
	union
	{
		uint16_t iZone;
		uint32_t ulZone;
	};
	int iX;
	int iY;
	union
	{
		int      iEventID;
		uint32_t m_HashEventOBJ;
	};
};

struct STR_COND_013
{
	int iNpcNo;
};

struct STR_COND_014
{
	uint16_t nSN;
	uint8_t  btOp;
};

struct STR_COND_015
{
	uint16_t nNumber1;
	uint16_t nNumber2;
};

struct STR_COND_016
{
	union
	{
		uint8_t  btWho;
		uint32_t ulWho;
	};
	uint32_t ulTime;
	uint32_t ulEndTime;
};

struct STR_COND_017
{
	STR_NPCVAR NpcVar1;
	STR_NPCVAR NpcVar2;
	uint8_t       btOp;
};

struct STR_COND_018
{
	uint8_t btDate;
	uint8_t btHour1;
	uint8_t btMin1;
	uint8_t btHour2;
	uint8_t btMin2;
};

struct STR_COND_019
{
	uint8_t btWeekDay;
	uint8_t btHour1;
	uint8_t btMin1;
	uint8_t btHour2;
	uint8_t btMin2;
};

struct STR_COND_020
{
	int iNo1;
	int iNo2;
};

struct STR_COND_021
{
	union
	{
		uint8_t  btSelObjType;
		uint32_t ulSelObjType;
	};
	int iRadius;
};

struct STR_COND_022
{
	uint16_t nX;
	uint16_t nY;
};

struct STR_COND_023
{
	uint8_t btReg;
};

struct STR_COND_024
{
	uint16_t nPOS;
	uint8_t  btOP;
};

struct STR_COND_025
{
	uint16_t nCONT;
	uint8_t  btOP;
};

struct STR_COND_026
{
	uint16_t nGRD;
	uint8_t  btOP;
};

struct STR_COND_027
{
	uint16_t nPOINT;
	uint8_t  btOP;
};

struct STR_COND_028
{
	uint16_t iMONEY;
	uint8_t  btOP;
};

struct STR_COND_029
{
	uint16_t nMemberCNT;
	uint8_t  btOP;
};

struct STR_COND_030
{
	uint16_t nSkill1;
	uint16_t nSkill2;
	uint8_t  btOP;
};
