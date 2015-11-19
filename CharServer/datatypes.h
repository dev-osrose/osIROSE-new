/*
    Open Source Rose Online Team - http://osroseon.to.md/
    note: the Server is develop with erose source server + eich source        
*/
#ifndef __ROSE_DATATYPES__
#define __ROSE_DATATYPES__
#include "../Common/sockets.h"
// List of clan members
struct CClanMembers
{
	uint32_t id;
	char     name[ 50 ];
	int      clan_rank;
};

// An item (for figuring out what a user is wearing)
struct CItem
{
	uint32_t owner;
	uint32_t itemnum;
	uint32_t itemtype;
	uint32_t refine;
	uint32_t durability;
	uint32_t lifespan;
};

// A single client character
struct CCharacter
{
	char              char_name[ 17 ];
	uint32_t          level;
	uint32_t          face;
	uint32_t          hairStyle;
	int               sex;
	uint32_t          classid;
	uint32_t          id;
	unsigned long int DeleteTime;
};

// List of friends
struct CFriendList
{
	int  id;
	char name[ 17 ];
};

// List of clan
struct CClans
{
	int                     id;
	int                     logo;
	int                     back;
	int                     grade;
	int                     cp;
	char                    name[ 17 ];
	char                    slogan[ 30 ];
	char                    news[ 260 ];
	vector< CClanMembers* > ClanMembers;
};

// Channel list
struct CChanels
{
	uint32_t id;
	char*    ip;
	uint32_t port;
	SOCKET   sock;
};
#endif
