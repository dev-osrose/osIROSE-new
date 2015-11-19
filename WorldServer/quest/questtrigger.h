// Props to ExJam for this code :D Ported to OSpRose by Drakia
class CQuestTrigger
{
	public:
	struct SQuestDatum
	{
		int      size;
		int      opcode;
		uint8_t* data;
	};

	std::string TriggerName;
	uint32_t    TriggerHash;

	uint8_t  CheckNext;
	uint32_t id;

	uint32_t      ConditionCount;
	SQuestDatum** Conditions;

	uint32_t      ActionCount;
	SQuestDatum** Actions;

	private:
};

struct STR_NPCVAR
{
	uint32_t iNpcNo;
	uint16_t nVarNo;
	uint16_t unused;
};

struct STR_ITEM_DATA
{
	uint32_t uiItemSN;
	uint32_t iWhere;
	uint32_t iRequestCnt;
	uint8_t  btOp;
};

struct STR_QUEST_DATA
{
	union
	{
		uint32_t iType;
		struct
		{
			short    m_wVarNO;
			uint16_t m_wVarTYPE;
		};
	};
	uint16_t nValue;
	uint8_t  btOp;
};

struct STR_ABIL_DATA
{
	uint32_t iType;
	int      iValue;
	uint8_t  btOp;
};

#define QUEST_FAILURE 6
#define QUEST_SUCCESS 5
template < class T >
int OperateValues( uint8_t btOp, T* value1PTR, T value2 )
{
	T value1 = *value1PTR;
	switch ( btOp )
	{
	case 0:
		return ( value1 == value2 );
	case 1:
		return ( value1 > value2 );
	case 2:
		return ( value1 >= value2 );
	case 3:
		return ( value1 < value2 );
	case 4:
		return ( value1 <= value2 );
	case 5:
		( *value1PTR ) = value2;
		return 1;
	case 6:
		( *value1PTR ) = value1 + value2;
		return 1;
	case 7:
		( *value1PTR ) = value1 - value2;
		return 1;
	case 9:
		( *value1PTR ) += 1;
		return 1;
	case 10:
		return ( value1 != value2 );
	default:
		Log( MSG_WARNING, "OperateValues unknown btOp='%u'", btOp );
		return 0;
	}
}
uint32_t GetRewardValue( uint32_t function, uint32_t amount, CPlayer* client, uint16_t nDupCNT );

#include "questconditions.h"
#include "questactions.h"
