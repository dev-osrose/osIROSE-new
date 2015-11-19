// Props to ExJam for this code :D
#define AICOND(cond) int F_AI_COND_ ## cond (class CWorldServer* server, class CCharacter* entity, uint8_t* raw)
typedef int (*fpAiCond)(class CWorldServer*, class CCharacter*, uint8_t*);
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
	//Hold or Attack (0x04 uint8_ts)
	uint8_t cNotfightOrDelay;	//Pos: 0x00
};

struct STR_AI_COND_001 {
	//Damage (0x08 uint8_ts)
	uint32_t iDamage;	//Pos: 0x00
	uint8_t cRecvOrGive;	//Pos: 0x04
};

struct STR_AI_COND_002 {
	//Check Near (1) (0x0c uint8_ts)
	int iDistance;	//Pos: 0x00
	union{uint8_t btIsAllied;	//Pos: 0x04
	uint16_t nIsAllied;};
	short nLevelDiff;	//Pos: 0x06
	short nLevelDiff2;	//Pos: 0x08
	uint16_t wChrNum;	//Pos: 0x0a
};

struct STR_AI_COND_003 {
	//Check Distance (1) (0x04 uint8_ts)
	int iDistance;	//Pos: 0x00
};

struct STR_AI_COND_004 {
	//Check Distance (2) (0x08 uint8_ts)
	int iDistance;	//Pos: 0x00
	uint8_t cMoreLess;	//Pos: 0x04
};

struct STR_AI_COND_005 {
	//Check AB (1) (0x0c uint8_ts)
	uint8_t cAbType;	//Pos: 0x00
	int iDiff;	//Pos: 0x04
	uint8_t cMoreLess;	//Pos: 0x08
};

struct STR_AI_COND_006 {
	//Check HP (0x08 uint8_ts)
	uint32_t wHP;	//Pos: 0x00
	uint8_t cMoreLess;	//Pos: 0x04
};

struct STR_AI_COND_007 {
	//Random Chance (0x04 uint8_ts)
	uint8_t cPercent;	//Pos: 0x00
};

struct STR_AI_COND_008 {
	//Check Near (2) (0x0c uint8_ts)
	uint32_t iDistance;	//Pos: 0x00
	uint16_t nLevelDiff;	//Pos: 0x04
	uint16_t nLevelDiff2;	//Pos: 0x06
	uint8_t btIsAllied;	//Pos: 0x08
};

struct STR_AI_COND_009 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_COND_010 {
	//Check AB (2) (0x04 uint8_ts)
	uint8_t cAbType;	//Pos: 0x00
	uint8_t cMoreLess;	//Pos: 0x01
};

struct STR_AI_COND_011 {
	//Check AB (3) (0x0c uint8_ts)
	uint8_t cAbType;	//Pos: 0x00
	uint32_t iValue;	//Pos: 0x04
	uint8_t cMoreLess;	//Pos: 0x08
};

struct STR_AI_COND_012 {
	//Check Time (1) (0x04 uint8_ts)
	uint8_t cWhen;	//Pos: 0x00
};

struct STR_AI_COND_013 {
	//Check Target (1) (0x04 uint8_ts)
	uint8_t btCheckTarget;	//Pos: 0x00
	uint8_t btStatusType;	//Pos: 0x01
	uint8_t btHave;	//Pos: 0x02
};

struct STR_AI_COND_014 {
	//Check Variable (1) (0x0c uint8_ts)
	uint8_t btVarIDX;	//Pos: 0x00
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_COND_015 {
	//Check Variable (2) (0x0c uint8_ts)
	uint16_t nVarIDX;	//Pos: 0x00
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_COND_016 {
	//Check Variable (3) (0x00 uint8_ts)
	uint16_t nVarIDX;	//Pos: 0x00
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_COND_017 {
	//Select NPC (0x04 uint8_ts)
	uint32_t iNpcNo;	//Pos: 0x00
};

struct STR_AI_COND_018 {
	//Check Distance (3) (0x08 uint8_ts)
	uint32_t iDistance;	//Pos: 0x00
	uint8_t btOp;	//Pos: 0x04
};

struct STR_AI_COND_019 {
	//Check Time (2) (0x08 uint8_ts)
	uint32_t ulTime;	//Pos: 0x00
	uint32_t ulEndTime;	//Pos: 0x04
};

struct STR_AI_COND_020 {
	//Check AB (4) (0x0c uint8_ts)
	union{uint8_t btAbType;	//Pos: 0x00
	uint32_t iAbType;};
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_COND_021 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_COND_022 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_COND_023 {
	//Check Time (3) (0x08 uint8_ts)
	uint32_t ulTime;	//Pos: 0x00
	uint32_t ulEndTime;	//Pos: 0x04
};

struct STR_AI_COND_024 {
	//Check Date Time (4) (0x08 uint8_ts)
	uint8_t btDate;	//Pos: 0x00
	uint8_t btHour1;	//Pos: 0x01
	uint8_t btMin1;	//Pos: 0x02
	uint8_t btHour2;	//Pos: 0x03
	uint8_t btMin2;	//Pos: 0x04
};

struct STR_AI_COND_025 {
	//Check Weekday Time (5) (0x08 uint8_ts)
	uint8_t btWeekDay;	//Pos: 0x00
	uint8_t btHour1;	//Pos: 0x01
	uint8_t btMin1;	//Pos: 0x02
	uint8_t btHour2;	//Pos: 0x03
	uint8_t btMin2;	//Pos: 0x04
};

struct STR_AI_COND_026 {
	//Check Position (0x04 uint8_ts)
	uint16_t nMin;	//Pos: 0x00
	uint16_t nMax;	//Pos: 0x02
};

struct STR_AI_COND_027 {
	//Check Near Character (0x10 uint8_ts)
	int iDistance;	//Pos: 0x00
	uint8_t btIsAllied;	//Pos: 0x04
	short nLevelDiff;	//Pos: 0x06
	short nLevelDiff2;	//Pos: 0x08
	short wChrNum;	//Pos: 0x0a
	uint8_t btOp;	//Pos: 0x0c
};

struct STR_AI_COND_028 {
	//Check Variable (4) (0x0c uint8_ts)
	uint16_t nVarIDX;	//Pos: 0x00
	uint8_t btOp;	//Pos: 0x08
	uint32_t iValue;	//Pos: 0x04
};

struct STR_AI_COND_029 {
	//Check Target (2) (0x04 uint8_ts)
	uint8_t btTargetType;	//Pos: 0x00
};

struct STR_AI_COND_030 {
	//Unknown (0x04 uint8_ts)
	uint32_t Unknown;	//Pos: 0x00
};
