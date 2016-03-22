#include "crosepacket.h"

namespace RoseCommon {

CRosePacket::CRosePacket(ePacketType mycommand, unsigned short mysize, unsigned short myunused) {
	Header.Command = mycommand;
	Header.Size = mysize;
	Header.Unused = myunused;
}

void CRosePacket::AddString(const std::string &value, bool nullTerminate) {
	for (auto &it : value)
		Add<uint8_t>(it);
	if (nullTerminate)
		Add<uint8_t>(0);
}

void CRosePacket::AddBytes(const std::vector<uint8_t> &value) {
	for (auto &it : value)
		Add<uint8_t>(it);
}

void CRosePacket::AddBytes(const uint8_t *value, size_t size) {
	for (size_t i = 0; i < size; ++i)
		Add<uint8_t>(value[i]);
}

std::string CRosePacket::GetString(uint16_t pos, size_t size) {
	return std::string(static_cast<char*>(&Data[pos]), size);
}

void CRosePacket::strcpy_safe(char *output, size_t charCount, const char *pSrc) {
	strncpy(output, pSrc, charCount);
	output[charCount - 1] = 0;
}

}
