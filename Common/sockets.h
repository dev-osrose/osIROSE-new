/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://osroseon.to.md

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    depeloped with Main erose/hrose source server + some change from the original eich source
*/
#ifndef __ROSE_SOCKETS__
#define __ROSE_SOCKETS__
#define USEIFO
#ifdef _WIN32

    #include <windows.h>
		#include <winsock2.h>
    //#define close closesocket
    #ifdef FD_SETSIZE
        #undef FD_SETSIZE
    #endif
    #define FD_SETSIZE	1024
#else
    #include <unistd.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <sys/ioctl.h>
    #include <cstdarg>
    #include <errno.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define PVOID void *
    #define ioctlsocket ioctl
    #define SOCKADDR sockaddr
    #define closesocket close
#endif
//#include <mysql/mysql.h>
#include <cstdio>
#include <cstdlib>
#include <cstdio>
#include <csignal>

#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <exception>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <cmath>
#include <map>

//C++11
#include <chrono>
#include <thread>
#include <mutex>
//-----

// MySQL
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
//-----

#include "ePacketType.h"
#include "log.h"
#include "rosecrypt.hpp"
#include "config.h"
#include "database/database.h"
// Just some defs for easier use

using std::cout;
using std::endl;
using std::system;
using std::vector;
using std::map;
using std::nothrow;
using std::string;
#define STARTPACKET(p,c,s) CPacket p; p.header.Command = c; p.header.Size = s;
#define SETBYTE(p,o,v) p.Buffer[o]=v;
#define SETWORD(p,o,v) *((unsigned short*)&p.Buffer[o])=v;
#define SETDWORD(p,o,v) *((unsigned*)&p.Buffer[o])=v;
#define SETQWORD(p,o,v) *((long long*)&p.Buffer[o])=v;
#define SETFLOAT(p,o,v) *((float*)&p.Buffer[o])=v;

#define BEGINPACKET(p,c) CPacket p; p.header.Command = c; p.header.Size = 6;
#define RESETPACKET(p,c) p.header.Command = c; p.header.Size = 6;
#define ADDBYTE(p,v) { p.Buffer[p.Size-6]=v; p.Size+=1; }
#define ADDWORD(p,v) { *((uint16_t*)&p.Buffer[p.Size-6])=v; p.Size+=2; }
#define ADDDWORD(p,v) { *((unsigned*)&p.Buffer[p.Size-6])=v; p.Size+=4; }
#define ADDQWORD(p,v) { *((uint64_t*)&p.Buffer[p.Size-6])=v; p.Size+=8; }
#define ADDFLOAT(p,v) { *((float*)&p.Buffer[p.Size-6])=v; p.Size+=4; }
#define ADDSTRING(p,v) { for(int q=0; q<(int)strlen((char*)v); q++) ADDBYTE(p,(v)[q]); }
#define GETBYTE(p,o) (p.Buffer[o])
#define GETWORD(p,o) *((WORD*)&p.Buffer[o])
#define GETDWORD(p,o) *((uint32_t*)&p.Buffer[o])
#define GETQWORD(p,o) *((uint64_t*)&p.Buffer[o])
#define GETFLOAT(p,o) *((float*)&p.Buffer[o])
//#define pi 3.14159265358979323846
#define r2d pi/180
#define ZONE_SIZE 6400
#define ENTITY_NPC 3

// Structures

// SQL Configuration
struct CROSEServerConfigSQL
{
    char*		pcServer;								// IP
    char*		pcDatabase;								// Database
    char*		pcUserName;								// UserName
    char*		pcPassword;								// Password
    int         pcPort;                    // port
};

// Server configuration
struct CROSEServerConfig
{
    // GLOBAL
    CROSEServerConfigSQL SQLServer;       // SQL Information
    bool        usethreads;               // Use Threads (in sockets)
    bool		Verbose;                  // print server information?
    unsigned	MaxConnections;           // Max connections
    unsigned	UserPerThread;	          // User per thread
    bool		AllowMultipleConnections; // Allow multiple connection from same ip?
    bool		StayInServerLoop;         // Server actived?
    uint32_t        ServerID;                 //id from this server
    uint32_t        ParentID;                 //id from the server to connect
    char*       ServerName;               //Server name
    uint32_t        ServerType;               //0 login - 1 server(char) - 2 channel(world)

    // LOGINSERVER
    unsigned int MinimumAccessLevel; // Minimum access level
    char*        LoginIP;            // LoginIP [external]
    unsigned int LoginPort;          // Login port
    unsigned int LoginPass;
    bool CreateLoginAccount;         // Create account in login if not exist?
    bool Testserver;                 // channel for test ?

    // CHARSEVER
    unsigned long int DeleteTime;    // Delete time
    char*        CharIP;             // CharIP
    unsigned int CharPort;           // Char port
    unsigned int CharPass;
    unsigned int CharsPort;

    // WORLDSERVER
    char*        WorldIP;
    unsigned int WorldPort;
    unsigned int WorldsPort;
    unsigned int WorldPass;
    uint32_t EXP_RATE;
    uint32_t DROP_RATE;
    uint32_t ZULY_RATE;
    uint32_t AUTOSAVE;
    uint32_t SAVETIME;
    uint32_t MapDelay;
    uint32_t VisualDelay;
    uint32_t WorldDelay;
    char* WELCOME_MSG;
    uint32_t DROP_TYPE;
    int MaxStat;
    int Partygap;
    int FairyMode;
    int FairyStay;
    int FairyWait;
    int FairyMax;
    int FairyTestMode;
    int PlayerDmg;
    int MonsterDmg;
    int Cfmode;
    int BlueChance;
    int StatChance;
    int SlotChance;
    int RefineChance;
    int Rare_Refine;
    int KillOnFail;

    // COMMAND LEVELS
    int Command_Go;
    int Command_Who;
    int Command_Who2;
    int Command_Broadcast;
    int Command_Mute;
    int Command_Tele;
    int Command_Level;
    int Command_LevelUp;
    int Command_TeleToMe;
    int Command_Save;
    int Command_Reload;
    int Command_Ann;
    int Command_SSpawn;
    int Command_Set;
    int Command_ESpawn;
    int Command_DSpawn;
    int Command_DelSpawn;
    int Command_Pak;
    int Command_Pak2;
    int Command_Pakm;
    int Command_Info;
    int Command_Exp;
    int Command_Mon;
    int Command_Kick;
    int Command_Job;
    int Command_Cha;
    int Command_Item;
    int Command_Drop;
    int Command_GiveZuly;
    int Command_Npc;
    int Command_GiveFairy;
    int Command_ManageFairy;
    int Command_ChangeFairyWait;
    int Command_ChangeFairyStay;
    int Command_ChangeFairyTestMode;
    int Command_Move;
    int Command_Goto;
    int Command_PlayerInfo;
    int Command_Give2;
    int Command_Ban;
    int Command_Ani;
    int Command_Summon;
    int Command_ReloadQuest;
    int Command_Shutdown;
    int Command_DQuest;
    int Command_IQuest;
    int Command_ShopType;
    int Command_Stat;
    int Command_KillInRange;
    int Command_GoToMap;
    int Command_Heal;
    int Command_ServerInfo;
    int Command_TargetInfo;
    int Command_Hide;
    int Command_Class;
    int Command_Monster;
    int Command_GlobalTime;
    int Command_Here;
    int Command_Face;
    int Command_Hair;
    int Command_Pvp;
    int Command_go;
    int Command_Convert;
    int Command_Rate;
    int Command_Moveto;
    int Command_Settime;
    int Command_Transx;
    int Command_Partylvl;
    int Command_Pdmg;
    int Command_Mdmg;
    int Command_Rules;
    int Command_Cfmode;

    char* Command_GlobalPrefix;

};


// Classes
// Base class
class CBaseSocket
{
public:
    CBaseSocket( );						// Constructor
    virtual ~CBaseSocket( );			// Destructor
    void	CloseSocket( void );		// Close Socket
    SOCKET					sock;				// This is our socket
    SOCKET          sckISC;             // Raven0123
    SOCKET          sckISCII;
    bool						isActive;			// Is this socket connected?
    bool            isserver;
    CCryptTable*		CryptTable;			// This is for decrypting incoming packets
#ifdef USE124
    char* csum;
    int csumlen;
#endif
    string          ClientIP;           // Client IP
    char            ClientSubNet[12];   // Lan Subnet
};
// Client class
class CClientSocket : public CBaseSocket
{
public:
    CClientSocket( );					// Client Constructor
    ~CClientSocket( );				    // Client Destructor
    bool ReceiveData( );                // Receive packet
    void SendPacket( CPacket *P );      // Send Packet
    void SendPacketCpy( CPacket *P );	// Send a packet without encrypting the orginal packet
    CCryptStatus				CryptStatus;			// Status of the encryption
    class CServerSocket* GS;	        // Pointer to Server class
    bool ISCThread(); // Raven0123
    struct sockaddr_in clientinfo;
    void* player;
#ifdef USE124
    char* ct;
#endif
private:
    unsigned short	PacketSize;		    // Size of the current packet
    unsigned short	PacketOffset;		// Current offset of the data
    unsigned char	Buffer[ 0xFFF ];    // Buffer to receive data into
    unsigned char	tempBuffer[0xFFF];
};

// Server class
class CServerSocket : public CBaseSocket
{
private:
    //void CryptISCPak( char* pak ); // Raven0123
    //void ISCThread(); // Raven0123
    //SOCKET sckISC; // Raven0123

public:
    void CryptISCPak( char* pak ); // Raven0123
    CServerSocket( );					 // Constructor
    ~CServerSocket( );					 // Destructor

    bool StartServer( );                 // Start Server
    void FillFDS( fd_set* fds );         // Create FDS table
    void HandleClients( fd_set* fds );   // Handle Clients
    void AddUser( SOCKET sock, sockaddr_in* ClientInfo, bool server ); // Add User
    void DisconnectClient( CClientSocket* thisclient );   // Disconnect Client
    bool DoSQL(char *Format, ...);

    // Virtual Functions
    virtual void LoadEncryption( );      //load encryption
    virtual void ServerLoop( );                  // Main Loop
    virtual CClientSocket* CreateClientSocket( ); // Create a client
    virtual void DeleteClientSocket( CClientSocket* thisclient ); // Delete client
    virtual bool OnServerReady( );       // Executed with server is ready
    virtual void OnServerStep( );        // Executed
    virtual void OnServerDie( );
    virtual bool OnReceivePacket( CClientSocket* thisclient, CPacket* P );
    virtual void ReceivedISCPacket( CPacket* pak ); // Raven0123
    virtual bool OnClientConnect( CClientSocket* thisclient );
    virtual void OnClientDisconnect( CClientSocket* thisclient );

    // Variables
    CROSEServerConfig	Config;	          // Hold server configuration
    int  maxfd;
    virtual bool Ping( );       // Used to ping the mysql server
    vector<CClientSocket*>      ClientList;
    uint32_t										ConnectedClients;
    unsigned short							port;
    struct sockaddr_in 					ain;
    struct sockaddr_in 					sain;

    pthread_t                   threads[65535];

    void SendISCPacket( CPacket* pak ); // Raven0123
};
PVOID ClientMainThread( PVOID ); // Handle clients
bool	InitWinSocket ( void ); // Init windows sockets (wsa)
void	CloseWinSocket( void ); // Close Windows sockets (wsa)
// -----------------------------------------------------------------------------------------

#endif
