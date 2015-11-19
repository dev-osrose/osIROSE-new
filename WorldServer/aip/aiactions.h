// Props to ExJam for this code :D
#define AIACT(act) int F_AI_ACT_ ## act (class CWorldServer* server, class CCharacter* entity, uint8_t* raw)
typedef int (*fpAiAct)(class CWorldServer*, class CCharacter*, uint8_t*);
#define GETAIACTDATA(act) STR_AI_ACT_ ## act * data = (STR_AI_ACT_ ## act *)raw;


AIACT(000);AIACT(001);AIACT(002);
AIACT(003);AIACT(004);AIACT(005);
AIACT(006);AIACT(007);AIACT(008);
AIACT(009);AIACT(010);AIACT(011);
AIACT(012);AIACT(013);AIACT(014);
AIACT(015);AIACT(016);AIACT(017);
AIACT(018);AIACT(019);AIACT(020);
AIACT(021);AIACT(022);AIACT(023);
AIACT(024);AIACT(025);AIACT(026);
AIACT(027);AIACT(028);AIACT(029);
AIACT(030);AIACT(031);AIACT(032);
AIACT(033);AIACT(034);AIACT(035);
AIACT(036);AIACT(037);

#define AI_ACT_COUNT 38

struct STR_AI_ACT_000 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_001 {
	//Do Action (0x04 bytes)
	uint8_t cAction;	//Pos: 0x00
};

struct STR_AI_ACT_002 {
	//Say LTB String (0x04 bytes)
	uint32_t iStrID;	//Pos: 0x00
};

struct STR_AI_ACT_003 {
	//Move (1) (0x08 bytes)
	uint32_t iDistance;	//Pos: 0x00
	uint8_t cSpeed;	//Pos: 0x04
};

struct STR_AI_ACT_004 {
	//Move (2) (0x08 bytes)
	uint32_t iDistance;	//Pos: 0x00
	uint8_t cSpeed;	//Pos: 0x04
};

struct STR_AI_ACT_005 {
	//Move (3) (0x04 bytes)
	uint8_t cSpeed;	//Pos: 0x00
};

struct STR_AI_ACT_006 {
	//Move (?) (0x08 bytes)
	uint32_t iDistance;	//Pos: 0x00
	uint8_t cAbType;	//Pos: 0x04
	uint8_t cMoreLess;	//Pos: 0x05
};

struct STR_AI_ACT_007 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_008 {
	//Move (4) (0x08 bytes)
	uint32_t iDistance;	//Pos: 0x00
	uint8_t cSpeed;	//Pos: 0x04
};

struct STR_AI_ACT_009 {
	//Spawn Monster (1) (0x04 bytes)
	uint16_t wMonster;	//Pos: 0x00
};

struct STR_AI_ACT_010 {
	//Spawn Monster (2) (0x04 bytes)
	uint16_t wMonster;	//Pos: 0x00
};

struct STR_AI_ACT_011 {
	//Spawn Monster (3) (0x08 bytes)
	int iDistance;	//Pos: 0x00
	int iNumOfMonster;	//Pos: 0x04
};

struct STR_AI_ACT_012 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_013 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_014 {
	//Unknown (0x04 bytes)
	int iDistance;	//Pos: 0x00
};

struct STR_AI_ACT_015 {
	//Retaliate ? (0x00 bytes)
};

struct STR_AI_ACT_016 {
	//Unknown (0x04 bytes)
	int iDistance;	//Pos: 0x00
};

struct STR_AI_ACT_017 {
	//Drop Item (0x10 bytes)
	uint16_t items[6];
	uint8_t btToOwner;	//Pos: 0x0c
};

struct STR_AI_ACT_018 {
	//Spawn Monster (4) (0x08 bytes)
	short cMonster;	//Pos: 0x00
	short wHowMany;	//Pos: 0x02
	int iDistance;	//Pos: 0x04
};

struct STR_AI_ACT_019 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_ACT_020 {
	//Spawn Monster (5) (0x08 bytes)
	uint16_t cMonster;	//Pos: 0x00
	uint8_t btPos;	//Pos: 0x02
	int iDistance;	//Pos: 0x04
};

struct STR_AI_ACT_021 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_ACT_022 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_ACT_023 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_ACT_024 {
	//Do Skill (0x08 uint8_ts)
	uint8_t btTarget;	//Pos: 0x00
	uint16_t nSkill;	//Pos: 0x02
	uint16_t nMotion;	//Pos: 0x04
};

struct STR_AI_ACT_025 {
	//Set Variable (1) (0x0c bytes)
	union{uint8_t btVarIDX;	//Pos: 0x00
	char cVarIdx[4];};
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_ACT_026 {
	//Set Variable (2) (0x00 uint8_ts)
	union{uint16_t nVarNo;	//Pos: 0x00
	char cVarNo[4];};
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_ACT_027 {
	//Set Variable (3) (0x00 uint8_ts)
	union{uint16_t nVarNo;	//Pos: 0x00
	char cVarNo[4];};
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_ACT_028 {
	//Shout/Ann LTB String (0x08 uint8_ts)
	union {
		uint8_t btMsgType;
		int iMsgType;
	};
	uint32_t iStrID;	//Pos: 0x04
};

struct STR_AI_ACT_029 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_ACT_030 {
	//Do Trigger (0x0d uint8_ts)
	uint16_t lenszTrigger;	//Pos: 0x00
	char* szTrigger;	//Pos: 0x02
};

struct STR_AI_ACT_031 {
	//Unknown (0x00 uint8_ts)
};

struct STR_AI_ACT_032 {
	//Zone (1) (0x04 uint8_ts)
	uint16_t nZoneNo;	//Pos: 0x00
	uint8_t btOnOff;	//Pos: 0x02
};

struct STR_AI_ACT_033 {
	//Zone (2) (0x00 uint8_ts)
};

struct STR_AI_ACT_034 {
	//Item (?) (0x04 uint8_ts)
	uint16_t nItemNum;	//Pos: 0x00
	uint16_t nCount;	//Pos: 0x02
};

struct STR_AI_ACT_035 {
	//Set Variable (4) (0x0c uint8_ts)
	union{uint16_t nVarIDX;	//Pos: 0x00
	char cVarIdx[4];};
	uint32_t iValue;	//Pos: 0x04
	uint8_t btOp;	//Pos: 0x08
};

struct STR_AI_ACT_036 {
	//Monster (1) (0x04 uint8_ts)
	uint16_t nMonster;	//Pos: 0x00
	uint8_t btMaster;	//Pos: 0x02
};

struct STR_AI_ACT_037 {
	//Monster (2) (0x0c uint8_ts)
	uint16_t nMonster;	//Pos: 0x00
	uint16_t nPos;	//Pos: 0x02
	uint32_t iDistance;	//Pos: 0x04
	uint8_t btMaster;	//Pos: 0x08
};
