/*
 * ePacketType.h
 *
 *  Created on: Nov 10, 2015
 *      Author: ctorres
 */

#ifndef EPACKETTYPE_H_
#define EPACKETTYPE_H_

enum ePacketType
{
	START_OFFSET = 0x700,
	LSV_ENCRYPT_REQ = 0x703,
	LSV_SERVER_REQ = 0x704,
	LSV_LOGIN_REQ = 0x708,
	LSV_SRV_SELECT_REQ = 0x70a,
	EPACKETMAX
};

//TODO: Add structures for each type of packet so we don't have to use those nasty Add functions

struct serverInfo
{
	uint16_t id;
	uint8_t pad1;
	uint16_t status;
	//name as string
};

struct pakLSVServerReq
{
	uint32_t	lServerID;
	uint8_t		bServerCount;
	// serverInfo sServers[10]; // There is a better way to do this, just can't think of one ATM. -Raven
};

// Packet information
struct CPacketHeader
{
	uint16_t Size;		// Packet size
	uint16_t Command;	// Packet command
	uint16_t Unused;	// unused?
};

struct CPacketBody
{
	union
	{
		unsigned char body[0];
		//TODO: Add structs here
		pakLSVServerReq s_pServerRequest; //TODO Change the naming convention later when we have more structures.
	};
};

struct CPacket
{
	unsigned short	Size;            // Packet size
	unsigned short	Command;         // Packet command
	unsigned short	Unused;	         // unused
	unsigned char	Buffer[0x400];	 // Packet data //0x600

	//CPacketHeader header;

	CPacket( unsigned short mycommand=0, unsigned short mysize=6, unsigned short myunused=0 );
	~CPacket( );
	void StartPacket( unsigned short mycommand, unsigned short mysize=6, unsigned short myunused=0 );
	void AddByte( unsigned char value );
	void AddWord( unsigned short value );
	void AddDWord( unsigned value );
	void AddQWord( unsigned long long value );
	void AddFloat( float value );
	void AddString( char* value );
	void SetByte( unsigned short pos, unsigned char value );
	void SetWord( unsigned short pos, unsigned short value );
	void SetDWord( unsigned short pos, unsigned value );
	void SetQWord( unsigned short pos, unsigned long long value );
	void SetFloat( unsigned short pos, float value );
	unsigned char GetByte( unsigned short pos );
	unsigned short GetWord( unsigned short pos );
	unsigned GetDWord( unsigned short pos );
	unsigned long long GetQWord( unsigned short pos );
	float GetFloat( unsigned short pos );
	char* GetString( unsigned short pos );
	// Functions added by Drakia
	template <class T> void Add( T value )
	{
		*((T*)&Buffer[Size]) = value;
		Size += sizeof(T);
	}
	void AddString( char* value, bool NullTerminate )
	{
		for (dword i = 0; i < strlen((const char*)value); i++)
		{
			Add<byte>(value[i]);
		}
		if (NullTerminate) Add<byte>(0);
	}
	template <class T> void AddString( char* value )
	{
		Add<T>(strlen((const char*)value));
		AddString(value, false);
	}
	void AddBytes( byte* value, dword len )
	{
		for (dword i = 0; i < len; i++)
			Add<byte>((byte)value[i]);
	}
};

#endif /* EPACKETTYPE_H_ */
