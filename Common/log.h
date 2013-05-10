/*
    Open Source Rose Online Team - http://osroseon.to.md/
    note: the Server is develop with erose source server + eich source        
*/
#ifndef __ROSE_LOGGING_FUNCTIONS__
#define __ROSE_LOGGING_FUNCTIONS__

// Log path
#ifdef _WIN32
#	define LOG_DIRECTORY "log/"
#endif

#ifdef __unix__
#	define LOG_DIRECTORY "/var/log/roseserver/"
#endif

#define LOG_LOGIN_SERVER		0x01
#define LOG_CHARACTER_SERVER	0x02
#define LOG_WORLD_SERVER     	0x03
#define LOG_SAME_FILE           0x04

// Log File names
extern unsigned char LOG_THISSERVER;
#define LOG_FILENAME_LOGINFILE  "loginserver.log"
#define LOG_FILENAME_CHARFILE   "charserver.log"
#define LOG_FILENAME_WORLDFILE  "worldserver.log"
#define LOG_DEFAULT_FILE        "server.log"
#define LOG_FILENAME_ERRORFILE  "errors.log"

#define LOG_LOGINPACKETS "loginpacket.log"
#define LOG_CHARPACKETS  "charpackets.log"
#define LOG_WORLDPACKETS "worldpackets.log"
#define LOG_DEFAULTPACKETS "packets.log"

// Error Types
enum msg_type {
	MSG_NONE,
	MSG_STATUS,
	MSG_SQL,
	MSG_INFO,
	MSG_NOTICE,
	MSG_WARNING,
	MSG_DEBUG,
	MSG_ERROR,
	MSG_FATALERROR,
	MSG_HACK,
	MSG_LOAD,
	MSG_SDEBUG,
	MSG_GMACTION,
	MSG_START,
	MSG_QUERY
};

// Logging function
void Log( msg_type flag, char* Format, ... );

#endif
