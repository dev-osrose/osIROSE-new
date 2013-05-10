#ifndef _ROSE_DATABASE_
#define _ROSE_DATABASE_
#include "../sockets.h"

class CDatabase
{
    public:
        CDatabase( char* , char*, char*, char*, unsigned int, MYSQL* );
        ~CDatabase( );

        char* Server;
        char* Username;
        char* Password;
        char* Database;
        unsigned int Port;
        unsigned int Timeout;
        time_t LastPing;
        int Connect( );
        int Reconnect( );
        void Disconnect( );
        bool QExecute( char *format,... );
        MYSQL_RES* QUse( char *format,... );
        MYSQL_RES* QStore( char *format,... );                
        bool DoSQL(char *Format, ...);
        void QFree( );
        bool Ping( );
        
        MYSQL* Mysql;
        MYSQL_RES* result;
        pthread_mutex_t SQLMutex;
};

#endif
