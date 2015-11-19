// Props to ExJam for this code :]
class CAip {
public:
	struct SAipDatum {
		int size;
		int opcode;
		uint8_t* data;
	};

	uint32_t AipID;

	uint32_t ConditionCount;
	SAipDatum** Conditions;

	uint32_t ActionCount;
	SAipDatum** Actions;
private:
};
#define AI_FAILURE 8
#define AI_SUCCESS 7
#include "aiconditions.h"
#include "aiactions.h"
