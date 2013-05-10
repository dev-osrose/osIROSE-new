#include "sockets.h"

CPacket::CPacket( unsigned short mycommand, unsigned short mysize, unsigned short myunused )
{
	Command = mycommand;
	Size = mysize;
	Unused = myunused;
}
CPacket::~CPacket( ) { }

void CPacket::StartPacket( unsigned short mycommand, unsigned short mysize, unsigned short myunused )
{
	Command = mycommand;
	Size = mysize;
	Unused = myunused;
}
void CPacket::AddByte( unsigned char value )
{
	Buffer[Size-6] = value;
	Size += 1;
}
void CPacket::AddWord( unsigned short value )
{
	*((unsigned short*)&Buffer[Size-6]) = value;
	Size += 2;
}
void CPacket::AddDWord( unsigned value )
{
	*((unsigned*)&Buffer[Size-6]) = value;
	Size += 4;
}
void CPacket::AddQWord( unsigned long long value )
{
	*((unsigned long long*)&Buffer[Size-6]) = value;
	Size += 8;
}
void CPacket::AddFloat( float value )
{
	*((float*)&Buffer[Size-6]) = value;
	Size += 4;
}
void CPacket::AddString( char* value )
{
	for(unsigned char c=0; c<(unsigned)strlen(value); c++)
		AddByte( value[c] );
}
void CPacket::SetByte( unsigned short pos, unsigned char value )
{
	Buffer[pos] = value;
}
void CPacket::SetWord( unsigned short pos, unsigned short value )
{
	*((unsigned short*)&Buffer[pos]) = value;
}
void CPacket::SetDWord( unsigned short pos, unsigned value )
{
	*((unsigned*)&Buffer[pos]) = value;
}
void CPacket::SetQWord( unsigned short pos, unsigned long long value )
{
	*((unsigned long long*)&Buffer[pos]) = value;
}
void CPacket::SetFloat( unsigned short pos, float value )
{
	*((float*)&Buffer[pos]) = value;
}
unsigned char CPacket::GetByte( unsigned short pos )
{
	return Buffer[pos];
}
unsigned short CPacket::GetWord( unsigned short pos )
{
	return *((unsigned short*)&Buffer[pos]);
}
unsigned CPacket::GetDWord( unsigned short pos )
{
	return *((unsigned*)&Buffer[pos]);
}
unsigned long long CPacket::GetQWord( unsigned short pos )
{
	return *((unsigned long long*)&Buffer[pos]);
}
float CPacket::GetFloat( unsigned short pos )
{
	return *((float*)&Buffer[pos]);
}

char* CPacket::GetString( unsigned short pos )
{
	return (char*)&Buffer[pos];
}
