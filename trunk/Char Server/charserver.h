/*
    Open Source Rose Online Team - http://osroseon.to.md/
    note: the Server is develop with erose source server + eich source        
*/
#ifndef __ROSE_SERVERS__
#define __ROSE_SERVERS__

#include <time.h>
#include "../common/sockets.h"
#include "datatypes.h"
#define ClearItem(i) { i.durability=0; i.itemnum=0; i.itemtype=0; i.lifespan=0; i.refine=0; }

// Clent class
class CCharClient : public CClientSocket
{
    public:
    	CCharClient ( );   // constructor
    	~CCharClient ( );  // destructor

        // User information
    	UINT userid;
        bool platinum;    
    	char username[17];
    	char password[33];
    	UINT accesslevel;
    	UINT channel;
	
    	// Character information
    	char charname[17];	
    	UINT clanid;	
    	int clan_rank;
    	UINT level;
    	UINT job;
        UINT charid;	
	
    	bool isLoggedIn;
    	bool returnedfromWS;
    	bool logout;	
    	vector<CFriendList*> FriendList;
};

// Server class
class CCharServer : public CServerSocket
{
    public:
    	CCharServer ( string );
    	~CCharServer( );
        CCharClient* CreateClientSocket( );
    	void DeleteClientSocket( CClientSocket* thisclient );
    	void LoadEncryption( );
    	void OnClientDisconnect( CClientSocket* thisclient );
    	bool OnServerReady( );    	
        void LoadConfigurations( char* );    	
    	bool OnReceivePacket( CClientSocket* thisclient, CPacket* P );
    	bool Ping( );
    
        // Extra functions
        CChanels* GetChannelByID( UINT id );
        CCharClient* GetClientByUserID( UINT id );
      	CCharClient* GetClientByID( UINT id );
    	CCharClient* GetClientByName( char *name );
    	CCharClient* GetClientByUserName( char *username );	
    	CClans* GetClanByID( int id );
        unsigned long int GetServerTime( );

        // Community functions
        bool ChangeMessengerStatus (CCharClient* thisclient, CCharClient* otherclient, int status);	
        bool pakMessengerManager   ( CCharClient* thisclient, CPacket* P );
        bool pakMessengerChat  ( CCharClient* thisclient, CPacket* P );
    
        // Clan Functions
        bool SendClanInfo (CCharClient* thisclient);
        bool ChangeClanStatus (CCharClient* thisclient, CCharClient* otherclient, int channel);
        bool SendToClanMembers( int clanid,CPacket* pak );
        bool UpdateClanWindow( int clanid );
        bool pakClanMembers ( CCharClient* thisclient );
        bool pakClanManager ( CCharClient* thisclient, CPacket* P );
        bool pakClanChat ( CCharClient* thisclient, CPacket* P );
        bool ClanLogout ( CCharClient* thisclient );
        bool pakUploadCM ( CCharClient* thisclient, CPacket* P );
        bool pakDownloadCM( CCharClient* thisclient, CPacket* P );
        bool pakClanIconTime( CCharClient* thisclient, CPacket* P );
    
        // General Packets
        bool pakUpdateLevel( CCharClient* thisclient, CPacket* P );
    	bool pakDoIdentify( CCharClient* thisclient, CPacket* P );
    	bool pakGetCharacters( CCharClient* thisclient, CPacket* P );
    	bool pakRequestWorld( CCharClient* thisclient, CPacket* P );
    	bool pakCreateChar( CCharClient* thisclient, CPacket* P );
    	bool pakDeleteChar( CCharClient* thisclient, CPacket* P );
    	bool pakWSReady ( CCharClient* thisclient, CPacket* P );
    	bool pakWSCharSelect ( CCharClient* thisclient, CPacket* P );
    	bool pakLoginConnected  ( CCharClient* thisclient, CPacket* P );
    	bool pakLoginDSClient( CCharClient* thisclient, CPacket* P );
    	bool pak7e5 ( CCharClient* thisclient, CPacket* P );
        bool pakChatrooms ( CCharClient* thisclient, CPacket* P );
            
        // Variables
        string filename;
        vector <CClans*> ClanList;
        vector <CChanels*> ChannelList;
        
            //Login
            SOCKET lsock;                // Socket to Login
        	char*    lct;                // Encryption table for login server  
       CDatabase* DB;          
};
void StartSignal( );
void StopSignal( );
void HandleSignal( int num );
extern class CCharServer* GServer;
#endif


