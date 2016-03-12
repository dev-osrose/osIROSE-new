#include "crosepacket.h"

namespace RoseCommon {

CRosePacket::CRosePacket(unsigned int mycommand, unsigned short mysize, unsigned short myunused) {
	Header.Command = (ePacketType)mycommand;
	Header.Size = mysize;
	Header.Unused = myunused;
}

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

void CRosePacket::AddBytes(const uint8_t *value, uint16_t size) {
	for (uint16_t i = 0; i < size; ++i)
		Add<uint8_t>(value[i]);
}

void CRosePacket::GetString(uint16_t pos, uint16_t size, char *outBuffer) {
	strcpy_safe(outBuffer, size, (char*)&Data[pos]);
}

void CRosePacket::GetBytes(uint16_t pos, uint16_t len, uint8_t* buffer) {
	memcpy(buffer, &Data[pos], len);
}

void CRosePacket::strcpy_safe(char *output, size_t charCount, const char *pSrc) {
	strncpy(output, pSrc, charCount);
	output[charCount - 1] = 0;
}

}
