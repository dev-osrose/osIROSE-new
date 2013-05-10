// Props to ExJam for this code :D
#define AICOND(cond) int F_AI_COND_ ## cond (class CWorldServer* server, class CCharacter* entity, byte* raw)
typedef int (*fpAiCond)(class CWorldServer*, class CCharacter*, byte*);
#define GETAICONDDATA(cond) STR_AI_COND_ ## cond * data = (STR_AI_COND_ ## cond *)raw;

AICOND(000);AICOND(001);AICOND(002);
AICOND(003);AICOND(004);AICOND(005);
AICOND(006);AICOND(007);AICOND(008);
AICOND(009);AICOND(010);AICOND(011);
AICOND(012);AICOND(013);AICOND(014);
AICOND(015);AICOND(016);AICOND(017);
AICOND(018);AICOND(019);AICOND(020);
AICOND(021);AICOND(022);AICOND(023);
AICOND(024);AICOND(025);AICOND(026);
AICOND(027);AICOND(028);AICOND(029);
AICOND(030);

#define AI_COND_COUNT 31

struct STR_AI_COND_000 {
	//Hold or Attack (0x04 bytes)
	byte cNotfightOrDelay;	//Pos: 0x00
};

struct STR_AI_COND_001 {
	//Damage (0x08 bytes)
	dword iDamage;	//Pos: 0x00
	byte cRecvOrGive;	//Pos: 0x04
};

struct STR_AI_COND_002 {
	//Check Near (1) (0x0c bytes)
	int iDistance;	//Pos: 0x00
	union{byte btIsAllied;	//Pos: 0x04
	word nIsAllied;};
	short nLevelDiff;	//Pos: 0x06
	short nLevelDiff2;	//Pos: 0x08
	word wChrNum;	//Pos: 0x0a
};

struct STR_AI_COND_003 {
	//Check Distance (1) (0x04 bytes)
	int iDistance;	//Pos: 0x00
};

struct STR_AI_COND_004 {
	//Check Distance (2) (0x08 bytes)
	int iDistance;	//Pos: 0x00
	byte cMoreLess;	//Pos: 0x04
};

struct STR_AI_COND_005 {
	//Check AB (1) (0x0c bytes)
	byte cAbType;	//Pos: 0x00
	int iDiff;	//Pos: 0x04
	byte cMoreLess;	//Pos: 0x08
};

struct STR_AI_COND_006 {
	//Check HP (0x08 bytes)
	dword wHP;	//Pos: 0x00
	byte cMoreLess;	//Pos: 0x04
};

struct STR_AI_COND_007 {
	//Random Chance (0x04 bytes)
	byte cPercent;	//Pos: 0x00
};

struct STR_AI_COND_008 {
	//Check Near (2) (0x0c bytes)
	dword iDistance;	//Pos: 0x00
	word nLevelDiff;	//Pos: 0x04
	word nLevelDiff2;	//Pos: 0x06
	byte btIsAllied;	//Pos: 0x08
};

struct STR_AI_COND_009 {
	//Unknown (0x00 bytes)
};

struct STR_AI_COND_010 {
	//Check AB (2) (0x04 bytes)
	byte cAbType;	//Pos: 0x00
	byte cMoreLess;	//Pos: 0x01
};

struct STR_AI_COND_011 {
	//Check AB (3) (0x0c bytes)
	byte cAbType;	//Pos: 0x00
	dword iValue;	//Pos: 0x04
	byte cMoreLess;	//Pos: 0x08
};

struct STR_AI_COND_012 {
	//Check Time (1) (0x04 bytes)
	byte cWhen;	//Pos: 0x00
};

struct STR_AI_COND_013 {
	//Check Target (1) (0x04 bytes)
	byte btCheckTarget;	//Pos: 0x00
	byte btStatusType;	//Pos: 0x01
	byte btHave;	//Pos: 0x02
};

struct STR_AI_COND_014 {
	//Check Variable (1) (0x0c bytes)
	byte btVarIDX;	//Pos: 0x00
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_COND_015 {
	//Check Variable (2) (0x0c bytes)
	word nVarIDX;	//Pos: 0x00
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_COND_016 {
	//Check Variable (3) (0x00 bytes)
	word nVarIDX;	//Pos: 0x00
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_COND_017 {
	//Select NPC (0x04 bytes)
	dword iNpcNo;	//Pos: 0x00
};

struct STR_AI_COND_018 {
	//Check Distance (3) (0x08 bytes)
	dword iDistance;	//Pos: 0x00
	byte btOp;	//Pos: 0x04
};

struct STR_AI_COND_019 {
	//Check Time (2) (0x08 bytes)
	dword ulTime;	//Pos: 0x00
	dword ulEndTime;	//Pos: 0x04
};

struct STR_AI_COND_020 {
	//Check AB (4) (0x0c bytes)
	union{byte btAbType;	//Pos: 0x00
	dword iAbType;};
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_COND_021 {
	//Unknown (0x00 bytes)
};

struct STR_AI_COND_022 {
	//Unknown (0x00 bytes)
};

struct STR_AI_COND_023 {
	//Check Time (3) (0x08 bytes)
	dword ulTime;	//Pos: 0x00
	dword ulEndTime;	//Pos: 0x04
};

struct STR_AI_COND_024 {
	//Check Date Time (4) (0x08 bytes)
	byte btDate;	//Pos: 0x00
	byte btHour1;	//Pos: 0x01
	byte btMin1;	//Pos: 0x02
	byte btHour2;	//Pos: 0x03
	byte btMin2;	//Pos: 0x04
};

struct STR_AI_COND_025 {
	//Check Weekday Time (5) (0x08 bytes)
	byte btWeekDay;	//Pos: 0x00
	byte btHour1;	//Pos: 0x01
	byte btMin1;	//Pos: 0x02
	byte btHour2;	//Pos: 0x03
	byte btMin2;	//Pos: 0x04
};

struct STR_AI_COND_026 {
	//Check Position (0x04 bytes)
	word nMin;	//Pos: 0x00
	word nMax;	//Pos: 0x02
};

struct STR_AI_COND_027 {
	//Check Near Character (0x10 bytes)
	int iDistance;	//Pos: 0x00
	byte btIsAllied;	//Pos: 0x04
	short nLevelDiff;	//Pos: 0x06
	short nLevelDiff2;	//Pos: 0x08
	short wChrNum;	//Pos: 0x0a
	byte btOp;	//Pos: 0x0c
};

struct STR_AI_COND_028 {
	//Check Variable (4) (0x0c bytes)
	word nVarIDX;	//Pos: 0x00
	byte btOp;	//Pos: 0x08
	dword iValue;	//Pos: 0x04
};

struct STR_AI_COND_029 {
	//Check Target (2) (0x04 bytes)
	byte btTargetType;	//Pos: 0x00
};

struct STR_AI_COND_030 {
	//Unknown (0x04 bytes)
	dword Unknown;	//Pos: 0x00
};
