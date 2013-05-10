/*
    Open Source Rose Online Team - http://osroseon.to.md/
    note: the Server is develop with erose source server + eich source        
*/
#ifndef __ROSE_DATATYPES__
#define __ROSE_DATATYPES__
#include "../common/sockets.h"
// List of clan members
struct CClanMembers
{
    UINT id;
    char name[50];
    int clan_rank;
};

// An item (for figuring out what a user is wearing)
struct CItem {
	UINT owner;
	UINT itemnum;
	UINT itemtype;
	UINT refine;
	UINT durability;
	UINT lifespan;
};

// A single client character
struct CCharacter {
	char char_name[17];
	UINT level;
	UINT face;
	UINT hairStyle;
	int sex;
	UINT classid;
	UINT id;
	unsigned long int DeleteTime;
};

// List of friends
struct CFriendList
{
    int id;
    char name[17];
};

// List of clan
struct CClans
{
    int id;
    int logo;
    int back;
    int grade;
    int cp;  
    char name[17];
    char slogan[30];
    char news[260];
    vector<CClanMembers*> ClanMembers;
};

// Channel list
struct CChanels
{
    UINT id;    
    char* ip;
    UINT port;    
    SOCKET sock;
};
#endif

