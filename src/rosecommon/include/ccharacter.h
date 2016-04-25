#ifndef __CCHARACTER_H__
#define __CCHARACTER_H__

#include "iserialize.h"
#include <string>

class CCharacter : public ISerialize {
	public:
		CCharacter() {}
		virtual ~CCharacter() {}

		uint8_t &race() { return race_; }
		uint8_t &stone() { return stone_; }
		uint8_t &hair() { return hair_; }
		uint8_t &face() { return face_; }
		uint16_t &zone() {return zone_; }
		std::string &name() {return name_; }
		uint8_t &charId() { return charId_; }
		uint8_t &runMode() { return runMode_; }
		uint8_t &rideMode() { return rideMode_; }
		uint8_t &weightRate() { return weightRate_; }
		uint16_t &positionZ() { return positionZ_; }
		uint32_t &secUntilDel() { return secUntilDel_; }
		uint16_t &level() { return level_; }
		uint16_t &job() { return job_; }
		uint8_t &platinium() { return platinium_; }

	protected:
		virtual void serialize(CRosePacket &os) const {}
		virtual void deserialize(CRosePacket &os) {}

	private:
		uint8_t			race_;
		uint8_t			stone_;
		uint8_t			hair_;
		uint8_t			face_;
		uint16_t		zone_; // map id
		std::string		name_;
		uint8_t			charId_;
		uint8_t			runMode_;
		uint8_t			rideMode_;
		uint8_t			weightRate_; // current amount of weight / max amount * 100
		uint16_t		positionZ_;
		uint32_t		secUntilDel_;
		uint16_t		level_;
		uint16_t		job_;
		uint8_t			platinium_;
};

#endif
