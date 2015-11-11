#ifndef _ROSE_DATABASE_
#define _ROSE_DATABASE_
#include "../sockets.h"

class CDatabase
{
    public:
        CDatabase( char* , char*, char*, char*, unsigned int );
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
        std::unique_ptr<sql::ResultSet> QUse( char *format,... );
        std::unique_ptr<sql::ResultSet> QStore( char *format,... );

		// Prepare statements
		sql::PreparedStatement* QPrepare(char *format);
		void QPrepareFree();
		//----------------
        bool DoSQL(char *Format, ...);
        void QFree( );
        bool Ping( );
        
        sql::Driver* driver;
        std::unique_ptr<sql::Connection> con;
        std::unique_ptr<sql::Statement> stmt;
		std::unique_ptr<sql::PreparedStatement> pstmt;
		std::mutex SQLMutex;
		std::mutex SQLPrepareMutex;

        //MYSQL* Mysql;
        //MYSQL_RES* result;
        //pthread_mutex_t SQLMutex;
};

#endif
