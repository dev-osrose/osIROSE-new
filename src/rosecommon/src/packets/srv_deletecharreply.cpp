#include "srv_deletecharreply.h"

namespace RoseCommon {

SrvDeleteCharReply::SrvDeleteCharReply() : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY) {
}

SrvDeleteCharReply::SrvDeleteCharReply(uint32_t remainingTime, std::string name) : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY), remainingTime_(remainingTime), name_(name) {
}

uint32_t &SrvDeleteCharReply::remainingTime() {
	return remainingTime_;
}

std::string &SrvDeleteCharReply::name() {
	return name_;
}

void SrvDeleteCharReply::pack() {
	*this << remainingTime_;
	*this << name_;
}

}