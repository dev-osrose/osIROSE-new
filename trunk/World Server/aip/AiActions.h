// Props to ExJam for this code :D
#define AIACT(act) int F_AI_ACT_ ## act (class CWorldServer* server, class CCharacter* entity, byte* raw)
typedef int (*fpAiAct)(class CWorldServer*, class CCharacter*, byte*);
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
	byte cAction;	//Pos: 0x00
};

struct STR_AI_ACT_002 {
	//Say LTB String (0x04 bytes)
	dword iStrID;	//Pos: 0x00
};

struct STR_AI_ACT_003 {
	//Move (1) (0x08 bytes)
	dword iDistance;	//Pos: 0x00
	byte cSpeed;	//Pos: 0x04
};

struct STR_AI_ACT_004 {
	//Move (2) (0x08 bytes)
	dword iDistance;	//Pos: 0x00
	byte cSpeed;	//Pos: 0x04
};

struct STR_AI_ACT_005 {
	//Move (3) (0x04 bytes)
	byte cSpeed;	//Pos: 0x00
};

struct STR_AI_ACT_006 {
	//Move (?) (0x08 bytes)
	dword iDistance;	//Pos: 0x00
	byte cAbType;	//Pos: 0x04
	byte cMoreLess;	//Pos: 0x05
};

struct STR_AI_ACT_007 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_008 {
	//Move (4) (0x08 bytes)
	dword iDistance;	//Pos: 0x00
	byte cSpeed;	//Pos: 0x04
};

struct STR_AI_ACT_009 {
	//Spawn Monster (1) (0x04 bytes)
	word wMonster;	//Pos: 0x00
};

struct STR_AI_ACT_010 {
	//Spawn Monster (2) (0x04 bytes)
	word wMonster;	//Pos: 0x00
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
	word items[6];
	byte btToOwner;	//Pos: 0x0c
};

struct STR_AI_ACT_018 {
	//Spawn Monster (4) (0x08 bytes)
	short cMonster;	//Pos: 0x00
	short wHowMany;	//Pos: 0x02
	int iDistance;	//Pos: 0x04
};

struct STR_AI_ACT_019 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_020 {
	//Spawn Monster (5) (0x08 bytes)
	word cMonster;	//Pos: 0x00
	byte btPos;	//Pos: 0x02
	int iDistance;	//Pos: 0x04
};

struct STR_AI_ACT_021 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_022 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_023 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_024 {
	//Do Skill (0x08 bytes)
	byte btTarget;	//Pos: 0x00
	word nSkill;	//Pos: 0x02
	word nMotion;	//Pos: 0x04
};

struct STR_AI_ACT_025 {
	//Set Variable (1) (0x0c bytes)
	union{byte btVarIDX;	//Pos: 0x00
	char cVarIdx[4];};
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_ACT_026 {
	//Set Variable (2) (0x00 bytes)
	union{word nVarNo;	//Pos: 0x00
	char cVarNo[4];};
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_ACT_027 {
	//Set Variable (3) (0x00 bytes)
	union{word nVarNo;	//Pos: 0x00
	char cVarNo[4];};
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_ACT_028 {
	//Shout/Ann LTB String (0x08 bytes)
	union {
		byte btMsgType;
		int iMsgType;
	};
	dword iStrID;	//Pos: 0x04
};

struct STR_AI_ACT_029 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_030 {
	//Do Trigger (0x0d bytes)
	word lenszTrigger;	//Pos: 0x00
	char* szTrigger;	//Pos: 0x02
};

struct STR_AI_ACT_031 {
	//Unknown (0x00 bytes)
};

struct STR_AI_ACT_032 {
	//Zone (1) (0x04 bytes)
	word nZoneNo;	//Pos: 0x00
	byte btOnOff;	//Pos: 0x02
};

struct STR_AI_ACT_033 {
	//Zone (2) (0x00 bytes)
};

struct STR_AI_ACT_034 {
	//Item (?) (0x04 bytes)
	word nItemNum;	//Pos: 0x00
	word nCount;	//Pos: 0x02
};

struct STR_AI_ACT_035 {
	//Set Variable (4) (0x0c bytes)
	union{word nVarIDX;	//Pos: 0x00
	char cVarIdx[4];};
	dword iValue;	//Pos: 0x04
	byte btOp;	//Pos: 0x08
};

struct STR_AI_ACT_036 {
	//Monster (1) (0x04 bytes)
	word nMonster;	//Pos: 0x00
	byte btMaster;	//Pos: 0x02
};

struct STR_AI_ACT_037 {
	//Monster (2) (0x0c bytes)
	word nMonster;	//Pos: 0x00
	word nPos;	//Pos: 0x02
	dword iDistance;	//Pos: 0x04
	byte btMaster;	//Pos: 0x08
};
