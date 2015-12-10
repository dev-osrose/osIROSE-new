/*
 * ePacketType.h
 *
 *  Created on: Nov 10, 2015
 *      Author: ctorres
 */

#ifndef EPACKETTYPE_H_
#define EPACKETTYPE_H_

typedef uint8_t  byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

// CS = Client -> server
// SC = server -> server
// SS = server -> server
// LC = Login -> server
// CC = Char -> Client
// WC = World -> client
enum struct ePacketType : uint16_t
{
	PAKSTART    = 0x700,
	PAKCS_ALIVE = PAKSTART,
	PAKSS_ERROR = PAKCS_ALIVE,
	PAKSS_ANNOUNCE_TEXT,
	PAKSW_ANNOUNCE_CHAT,
	PAKCS_ACCEPT_REQ,
	PAKCS_CHANNEL_LIST_REQ,
	PAKLC_CHANNEL_LIST_REPLY = PAKCS_CHANNEL_LIST_REQ,

	PAKCS_LOGOUT_REQ   = 0x707,
	PAKWC_LOGOUT_REPLY = PAKCS_LOGOUT_REQ,
	PAKCS_LOGIN_REQ,
	PAKLC_LOGIN_REPLY = PAKCS_LOGIN_REQ,
	PAKGC_LOGIN_REPLY,
	PAKCS_SRV_SELECT_REQ = 0x70a,
	PAKLC_SRV_SELECT_REQ = PAKCS_SRV_SELECT_REQ,

	PAKCS_JOIN_SERVER_REQ,
	PAKSC_JOIN_SERVER_REPLY,

	PAKCC_SWITCH_SERVER = 0x711,
	PAKCS_CHAR_LIST_REQ,
	PAKCC_CHAR_LIST_REPLY = PAKCS_CHAR_LIST_REQ,
	PAKCS_CREATE_CHAR_REQ,
	PAKCC_CREATE_CHAR_REPLY = PAKCS_CREATE_CHAR_REQ, // 0x713
	PAKCS_DELETE_CHAR_REQ,
	PAKCC_DELETE_CHAR_REPLY = PAKCS_DELETE_CHAR_REQ,
	PAKCS_SELECT_CHAR_REQ,
	PAKWC_SELECT_CHAR_REPLY = PAKCS_SELECT_CHAR_REQ,

	PAKWC_INVENTORY_DATA,
	PAKWC_SETMONEY,
	PAKWC_SET_ITEM,
	PAKWC_SERVER_DATA, // 0x719

	PAKWC_QUEST_DATA = 0x71b,
	PAKCS_CHANGE_CHAR_REQ,
	PAKCC_CHAN_CHAR_REPLY = PAKCS_CHANGE_CHAR_REQ,
	PAKWC_SET_MONEY,
	PAKWC_QUEST_REWARD_MONEY,
	PAKWC_QUEST_REWARD_ITEM,
	PAKWC_QUEST_REWARD_ADD_STAT,
	PAKWC_QUEST_REWARD_SET_STAT,
	PAKCS_CANCEL_LOGOUT,
	PAKWC_QUEST_UPDATE,

	PAKCS_QUEST_DATA_REQ   = 0x730,
	PAKWC_QUEST_DATA_REPLY = PAKCS_QUEST_DATA_REQ,

	PAKCS_CHANGE_MAP_REQ   = 0x753,
	PAKWC_CHANGE_MAP_REPLY = PAKCS_CHANGE_MAP_REQ,

	EPACKETMAX
};

//inline bool operator< (const ePacketType& lhs, const ePacketType& rhs){ return static_cast<int32_t>(lhs) < static_cast<int32_t>(rhs); }
//inline bool operator> (const ePacketType& lhs, const ePacketType& rhs){return rhs < lhs;}
//inline bool operator<=(const ePacketType& lhs, const ePacketType& rhs){return !(lhs > rhs);}
//inline bool operator>=(const ePacketType& lhs, const ePacketType& rhs){return !(lhs < rhs);}
inline bool operator! (const ePacketType& rhs){return static_cast<int32_t>(rhs) == 0;}
inline bool operator!=(const uint32_t& lhs, const ePacketType& rhs){return (lhs != static_cast<int32_t>(rhs));}

//TODO: Add structures for each type of packet so we don't have to use those nasty Add functions

// Packet information
struct sPacketHeader
{
	uint16_t Size;    // Packet size
	ePacketType Command; // Packet command
	uint16_t Unused;  // unused?
};

struct channelInfo
{
	uint8_t  ChannelID;
	uint8_t  pad1;
	uint8_t  pad2;
	uint16_t Status;
	uint32_t Right;
	//Channel Name as string
};

struct pakChannel_List : public sPacketHeader
{
	uint32_t lServerID;
	uint8_t  bServerCount;
	// channelInfo sServers[]; // There is a better way to do this, just can't think of one ATM. -Raven
};

struct pakChannelList_Req : public sPacketHeader
{
	uint32_t lServerID;
};

struct pakLoginReply : public sPacketHeader
{
	uint8_t  Result;
	uint16_t Right;
	uint16_t Unknown;
};

struct CPacket
{
	//unsigned short	Size;            // Packet size
	//unsigned short	Command;         // Packet command
	//unsigned short	Unused;	         // unused
	//unsigned char		Buffer[0x7FF];	 // Packet data

	union
	{
		struct
		{
			sPacketHeader Header;
			uint8_t       Data[ 0x400 - 6 ];
		};
		uint8_t Buffer[ 0x400 ];

		pakLoginReply      pLoginReply;
		pakChannelList_Req pChannelListReq;
		pakChannel_List    pChannelList;
	};

	CPacket( unsigned short mycommand = 0, unsigned short mysize = 6, unsigned short myunused = 0 )
	{
		Header.Command = (ePacketType)mycommand;
		Header.Size    = mysize;
		Header.Unused  = myunused;
	}

	CPacket( ePacketType mycommand, unsigned short mysize = 6, unsigned short myunused = 0 )
	{
		Header.Command = mycommand;
		Header.Size    = mysize;
		Header.Unused  = myunused;
	}

	~CPacket( )
	{
	}

	void  StartPacket( unsigned short mycommand, unsigned short mysize = 6, unsigned short myunused = 0 );
	char* GetString( unsigned short pos );
	//void               AddByte( unsigned char value );
	//void               AddWord( unsigned short value );
	//void               AddDWord( unsigned value );
	//void               AddQWord( unsigned long long value );
	//void               AddFloat( float value );
	//void               SetByte( unsigned short pos, unsigned char value );
	//void               SetWord( unsigned short pos, unsigned short value );
	//void               SetDWord( unsigned short pos, unsigned value );
	//void               SetQWord( unsigned short pos, unsigned long long value );
	//void               SetFloat( unsigned short pos, float value );
	unsigned char      GetByte( unsigned short pos );
	unsigned short     GetWord( unsigned short pos );
	unsigned           GetDWord( unsigned short pos );
	unsigned long long GetQWord( unsigned short pos );
	float              GetFloat( unsigned short pos );

	// Functions added by Drakia
	template < class T >
	void Add( T value )
	{
		*( (T*)&Data[ Header.Size ] ) = value;
		Header.Size += sizeof( T );
	}
	void AddString( const char* value, bool NullTerminate )
	{
		for ( uint32_t i = 0; i < strlen( (const char*)value ); i++ )
		{
			Add< uint8_t >( value[ i ] );
		}
		if ( NullTerminate )
			Add< uint8_t >( 0 );
	}
	template < class T >
	void AddString( const char* value )
	{
		Add< T >( strlen( (const char*)value ) );
		AddString( value, false );
	}
	void AddBytes( uint8_t* value, uint32_t len )
	{
		for ( uint32_t i = 0; i < len; i++ )
			Add< uint8_t >( (uint8_t)value[ i ] );
	}
	//*/
};

#endif /* EPACKETTYPE_H_ */
