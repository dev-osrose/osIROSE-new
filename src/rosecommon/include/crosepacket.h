#ifndef _CROSEPACKET_H_
#define _CROSEPACKET_H_

#include "ePacketType.h"
#include "iscpackets.pb.h"

using namespace iscPacket;

struct CRosePacket
{
	union
	{
		struct
		{
			sPacketHeader Header;
			uint8_t       Data[ MAX_PACKET_SIZE - 6 ];
		};
		uint8_t Buffer[ MAX_PACKET_SIZE ];

		pakEncryptionRequest pEncryptReq;
		pakLoginReply      pLoginReply;
		pakChannelList_Req pChannelListReq;
		pakChannel_List    pChannelList;
	};

	CRosePacket( unsigned short mycommand = 0, unsigned short mysize = 6, unsigned short myunused = 0 )
	{
		Header.Command = (ePacketType)mycommand;
		Header.Size    = mysize;
		Header.Unused  = myunused;
	}

	CRosePacket( ePacketType mycommand, unsigned short mysize = 6, unsigned short myunused = 0 )
	{
		Header.Command = mycommand;
		Header.Size    = mysize;
		Header.Unused  = myunused;
	}

	~CRosePacket( )
	{
	}

	// Functions added by Drakia
	template < class T >
	void Add( T value )
	{
		*( (T*)&Buffer[ Header.Size ] ) = value;
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
	void AddBytes( uint8_t* value, uint16_t len )
	{
		for ( uint16_t i = 0; i < len; i++ )
			Add< uint8_t >( value[ i ] );
	}

	// Functions added by Raven
	template < class T >
	T Get( uint16_t pos )
	{
		return Data[pos];
	}

	void GetString ( uint16_t pos, uint16_t size, char* outbuffer )
	{
		strcpy_safe( outbuffer, size, (char*)&Data[pos] );
	}
	void GetBytes ( uint16_t pos, uint16_t len, uint8_t* outbuffer )
	{
		memcpy( outbuffer, &Data[pos], len );
	}

private:
	// This is only here until g++ adds c++11 std::strcpy_s
	template <size_t charCount>
	void strcpy_safe(char (&output)[charCount], const char* pSrc)
	{
	        strncpy(output, pSrc, charCount);
	        output[charCount - 1] = 0;
	}

	void strcpy_safe(char* output, size_t charCount, const char* pSrc)
	{
        	strncpy(output, pSrc, charCount);
	        output[charCount - 1] = 0;
	}
};

#endif
