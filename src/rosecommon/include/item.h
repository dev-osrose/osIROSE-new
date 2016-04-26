#ifndef __ITEM_H__
#define __ITEM_H__

#include "iserialize.h"

class Item : public ISerialize {
	public:
		Item() {}
		Item(uint16_t id, uint16_t gemOp, bool socket, uint8_t grade) : id_(id), gemOp_(gemOp), socket_(socket), grade_(grade) {}
		virtual ~Item() {}

	protected:
		virtual void serialize(CRosePacket &os) const {
			uint32_t data = ((grade_ & 0xF) << 20) | ((socket & 0x1) << 19) | ((gemOp_ & 0x1FF) << 10) | (id_ & 0x3FF);
			os << data;
		}
		virtual void deserialize(CRosePacket &os) {
			uint32_t data;
			oss >> data;
			id_ = data & 0x3FF;
			gemOp_ = (data >> 10) & 0x1FF;
			socket_ = (data >> 19) & 0x1;
			grade_ = (data >> 20) & 0xF;
		}

	private:
		uint16_t	id_;
		uint16_t	gemOp_;
		bool		socket_;
		uint8_t		grade_;
};

#endif
