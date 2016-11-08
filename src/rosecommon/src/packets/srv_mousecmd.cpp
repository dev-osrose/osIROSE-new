#include "srv_mousecmd.h"

namespace RoseCommon {

SrvMouseCmd::SrvMouseCmd() : CRosePacket(ePacketType::PAKWC_MOUSE_CMD) {
}

SrvMouseCmd::SrvMouseCmd(uint16_t sourceId, uint16_t destId, uint16_t dist, float x, float y, uint16_t z) : CRosePacket(ePacketType::PAKWC_MOUSE_CMD), sourceId_(sourceId), destId_(destId), dist_(dist), x_(x), y_(y), z_(z) {
}

uint16_t &SrvMouseCmd::sourceId() {
	return sourceId_;
}

uint16_t &SrvMouseCmd::destId() {
	return destId_;
}

uint16_t &SrvMouseCmd::dist() {
	return dist_;
}

float &SrvMouseCmd::x() {
	return x_;
}

float &SrvMouseCmd::y() {
	return y_;
}

uint16_t &SrvMouseCmd::z() {
	return z_;
}

void SrvMouseCmd::pack() {
	*this << sourceId_;
	*this << destId_;
	*this << dist_;
	*this << x_;
	*this << y_;
	*this << z_;
}

}