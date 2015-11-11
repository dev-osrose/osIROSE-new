/*
 * ePacketType.h
 *
 *  Created on: Nov 10, 2015
 *      Author: ctorres
 */

#ifndef EPACKETTYPE_H_
#define EPACKETTYPE_H_
/*
typedef unsigned __int8			byte;
typedef unsigned __int16		word;
typedef unsigned __int32		dword;
typedef unsigned __int64		qword;
typedef char*					strings;*/

enum ePacketType
{
	PAKSTART = 0x700,
	PAKCS_ALIVE = PAKSTART,
	PAKSS_ERROR = PAKCS_ALIVE,
	PAKSS_ANNOUNCE_TEXT,
	PAKSW_ANNOUNCE_CHAT,
	PAKCL_ACCEPT_REQ = 0x703,
	PAKSL_CHANNEL_LIST_REPLY = 0x704,

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
	///*
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
		for (uint32_t i = 0; i < strlen((const char*)value); i++)
		{
			Add<uint8_t>(value[i]);
		}
		if (NullTerminate) Add<uint8_t>(0);
	}
	template <class T> void AddString( char* value )
	{
		Add<T>(strlen((const char*)value));
		AddString(value, false);
	}
	void AddBytes( uint8_t* value, uint32_t len )
	{
		for (uint32_t i = 0; i < len; i++)
			Add<uint8_t>((uint8_t)value[i]);
	}
	//*/
};

#endif /* EPACKETTYPE_H_ */
