#include "sockets.h"

CPacket::CPacket( unsigned short mycommand, unsigned short mysize, unsigned short myunused )
{
	Header.Command = mycommand;
	Header.Size    = mysize;
	Header.Unused  = myunused;
}

CPacket::CPacket( ePacketType mycommand, unsigned short mysize, unsigned short myunused )
{
	Header.Command = (uint16_t)mycommand;
	Header.Size    = mysize;
	Header.Unused  = myunused;
}
CPacket::~CPacket( )
{
}

void CPacket::StartPacket( unsigned short mycommand, unsigned short mysize, unsigned short myunused )
{
	Header.Command = mycommand;
	Header.Size    = mysize;
	Header.Unused  = myunused;
}
/*
void CPacket::AddByte( unsigned char value )
{
	Buffer[ Header.Size - 6 ] = value;
	Header.Size += 1;
}
void CPacket::AddWord( unsigned short value )
{
	*( (unsigned short*)&Buffer[ Header.Size - 6 ] ) = value;
	Header.Size += 2;
}
void CPacket::AddDWord( unsigned value )
{
	*( (unsigned*)&Buffer[ Header.Size - 6 ] ) = value;
	Header.Size += 4;
}
void CPacket::AddQWord( unsigned long long value )
{
	*( (unsigned long long*)&Buffer[ Header.Size - 6 ] ) = value;
	Header.Size += 8;
}
void CPacket::AddFloat( float value )
{
	*( (float*)&Buffer[ Header.Size - 6 ] ) = value;
	Header.Size += 4;
}
void CPacket::SetByte( unsigned short pos, unsigned char value )
{
	Buffer[ pos ] = value;
}
void CPacket::SetWord( unsigned short pos, unsigned short value )
{
	*( (unsigned short*)&Buffer[ pos ] ) = value;
}
void CPacket::SetDWord( unsigned short pos, unsigned value )
{
	*( (unsigned*)&Buffer[ pos ] ) = value;
}
void CPacket::SetQWord( unsigned short pos, unsigned long long value )
{
	*( (unsigned long long*)&Buffer[ pos ] ) = value;
}
void CPacket::SetFloat( unsigned short pos, float value )
{
	*( (float*)&Buffer[ pos ] ) = value;
}
//*/

unsigned char CPacket::GetByte( unsigned short pos )
{
	return Buffer[ pos ];
}
unsigned short CPacket::GetWord( unsigned short pos )
{
	return *( (unsigned short*)&Buffer[ pos ] );
}
unsigned CPacket::GetDWord( unsigned short pos )
{
	return *( (unsigned*)&Buffer[ pos ] );
}
unsigned long long CPacket::GetQWord( unsigned short pos )
{
	return *( (unsigned long long*)&Buffer[ pos ] );
}
float CPacket::GetFloat( unsigned short pos )
{
	return *( (float*)&Buffer[ pos ] );
}

char* CPacket::GetString( unsigned short pos )
{
	return (char*)&Buffer[ pos ];
}
//*/

void CROSEServerConfig::LoadConfig( char* _file, bool _createConfig )
{
	try
	{
		// Database
		ReadWriteIniKeyValueStringA( "DATABASE", "mysql_host", "localhost", _file, _createConfig, &SQLServer.pcServer );
		ReadWriteIniKeyValueStringA( "DATABASE", "mysql_database", "roseon_beta", _file, _createConfig, &SQLServer.pcDatabase );
		ReadWriteIniKeyValueStringA( "DATABASE", "mysql_user", "root", _file, _createConfig, &SQLServer.pcUserName );
		ReadWriteIniKeyValueStringA( "DATABASE", "mysql_pass", "root", _file, _createConfig, &SQLServer.pcPassword );
		ReadWriteIniKeyValueInt( "DATABASE", "mysql_port", 3306, _file, _createConfig, &SQLServer.pcPort );

		// Server
		ReadWriteIniKeyValueInt( "SERVERDATA", "ServerId", 0, _file, _createConfig, (int*)&ServerID );
		ReadWriteIniKeyValueStringA( "SERVERDATA", "ServerIp", "127.0.0.1", _file, _createConfig, &WorldIP );
		ReadWriteIniKeyValueInt( "SERVERDATA", "AccessLevel", 100, _file, _createConfig, (int*)&MinimumAccessLevel );
		ReadWriteIniKeyValueInt( "SERVERDATA", "ParentId", 0, _file, _createConfig, (int*)&ParentID );
		ReadWriteIniKeyValueInt( "SERVERDATA", "MaxConnections", 100, _file, _createConfig, (int*)&MaxConnections );
		usethreads = ReadWriteIniKeyValueInt( "SERVERDATA", "UseThreads", 0, _file, _createConfig ) == 0 ? false : true;
		ReadWriteIniKeyValueInt( "SERVERDATA", "ServerMode", 0, _file, _createConfig, (int*)&ServerMode );

		// Internal Server Connection
		ReadWriteIniKeyValueStringA( "ISCDATA", "CharIp", "127.0.0.1", _file, _createConfig, &CharIP );
		ReadWriteIniKeyValueInt( "ISCDATA", "CharPort", 29110, _file, _createConfig, (int*)&CharsPort );
		ReadWriteIniKeyValueInt( "ISCDATA", "WorldPort", 29210, _file, _createConfig, (int*)&WorldsPort );

		// Login Server
		ReadWriteIniKeyValueStringA( "LOGINSERVER", "LoginName", "Server", _file, _createConfig, &LoginName );
		ReadWriteIniKeyValueInt( "LOGINSERVER", "LoginPort", 29000, _file, _createConfig, (int*)&LoginPort );
		CreateLoginAccount = ReadWriteIniKeyValueInt( "LOGINSERVER", "CreateLoginAccount", 0, _file, _createConfig ) == 0 ? false : true;
		Testserver         = ReadWriteIniKeyValueInt( "LOGINSERVER", "TestServer", 0, _file, _createConfig ) == 0 ? false : true;

		// Char Server
		ReadWriteIniKeyValueStringA( "CHARSERVER", "CharName", "Server", _file, _createConfig, &CharName );
		ReadWriteIniKeyValueInt( "CHARSERVER", "DeleteTime", 36000, _file, _createConfig, (int*)&DeleteTime );
		ReadWriteIniKeyValueInt( "CHARSERVER", "CharPort", 29100, _file, _createConfig, (int*)&CharPort );

		// World Server
		ReadWriteIniKeyValueStringA( "WORLDSERVER", "WorldName", "Server", _file, _createConfig, &WorldName );
		ReadWriteIniKeyValueStringA( "WORLDSERVER", "motd", "Welcome to Rose Online", _file, _createConfig, &WELCOME_MSG );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "WorldPort", 29200, _file, _createConfig, (int*)&WorldPort );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "drop_type", 2, _file, _createConfig, (int*)&DROP_TYPE );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "autosave", 0, _file, _createConfig, (int*)&AUTOSAVE );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "savetime", 3600, _file, _createConfig, (int*)&SAVETIME );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "mapdelay", 10, _file, _createConfig, (int*)&MapDelay );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "worlddelay", 200, _file, _createConfig, (int*)&WorldDelay );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "visualdelay", 500, _file, _createConfig, (int*)&VisualDelay );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "partygap", 10, _file, _createConfig, (int*)&Partygap );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "maxstat", 254, _file, _createConfig, (int*)&MaxStat );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "fairy", 1, _file, _createConfig, (int*)&FairyMode );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "fairystay", 20, _file, _createConfig, (int*)&FairyStay );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "fairywait", 15, _file, _createConfig, (int*)&FairyWait );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "fairymax", 0, _file, _createConfig, (int*)&FairyMax );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "fairytestmode", 1, _file, _createConfig, (int*)&FairyTestMode );
		ReadWriteIniKeyValueInt( "WORLDSERVER", "cfmode", 0, _file, _createConfig, (int*)&Cfmode );

		// Passwords
		ReadWriteIniKeyValueInt( "PASSWORDS", "loginpass", 123456, _file, _createConfig, (int*)&LoginPass );
		ReadWriteIniKeyValueInt( "PASSWORDS", "charpass", 123456, _file, _createConfig, (int*)&CharPass );
		ReadWriteIniKeyValueInt( "PASSWORDS", "worldpass", 123456, _file, _createConfig, (int*)&WorldPass );
	}
	catch ( ... )
	{
		Log( MSG_FATALERROR, "Error parsing configuration file" );
	}
}