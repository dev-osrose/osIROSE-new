#include "database.h"
#include "../log.h"

// constructor
CDatabase::CDatabase( char* server , char* username, char* password, char* database, unsigned int port , MYSQL* mysql)
{
    Server = server;
    Username = username;
    Password = password;
    Database = database;
    Port = port;    
    Mysql = mysql;
    LastPing = time( NULL );
    SQLMutex = PTHREAD_MUTEX_INITIALIZER;
    mysql_init( Mysql ); 
}

// deconstructor
CDatabase::~CDatabase( )
{
}

// disconnect from mysql
void CDatabase::Disconnect( )
{
    mysql_close( Mysql );
}

int CDatabase::Connect( )
{
    Log( MSG_INFO, "Connecting to MySQL" );
	if(!mysql_real_connect( Mysql, Server, Username, Password, Database,Port, NULL, 0 ))
	{
		Log( MSG_FATALERROR, "Error connecting to MySQL server: %s\n", mysql_error( Mysql ) );
		return -1;
    }    
  // Get timeout so we know when to ping - Drakia
  MYSQL_RES *result = QStore("SELECT @@wait_timeout");
  MYSQL_ROW row = mysql_fetch_row( result );
  Timeout = atoi(row[0]) - 60; // Set the timeout to 1 minute before the connection will timeout
  QFree( );
  Log( MSG_INFO, "MySQL Ping Timeout: %d seconds", Timeout );
  return 0;
}

// reconnect to mysql
int CDatabase::Reconnect( )
{
    Log( MSG_INFO, "Reconnecting to Mysql" ); 
	if(!mysql_real_connect( Mysql, Server, Username, Password, Database,Port, NULL, 0 ))
	{
		Log( MSG_FATALERROR, "Error reconnecting to MySQL server: %s\n", mysql_error( Mysql ) );
		return -1;
    }
    return 0;
}

// execute query
bool CDatabase::QExecute( char *Format,... )
{
    bool Qfail = true;
    char query[80000];
	va_list ap; 
    va_start( ap, Format );
	vsprintf( query, Format, ap ); 
	va_end  ( ap );    
    Log( MSG_QUERY, query );	
    pthread_mutex_lock( &SQLMutex );
    while(Qfail)
    {
        if(mysql_query( Mysql, query )!=0)
        {
            Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
            if(Reconnect( )==-1)
            {
                Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
                pthread_mutex_unlock( &SQLMutex );        
                return false;
            }
            else Qfail = false;
        }
        else Qfail = false;
    }
    pthread_mutex_unlock( &SQLMutex );            
    return true;    
}

MYSQL_RES* CDatabase::QStore( char *Format, ...)
{
    bool Qfail = true;    
    char query[80000];
	va_list ap; 
    va_start( ap, Format );
	vsprintf( query, Format, ap ); 
	va_end  ( ap );  
    result = NULL;
    Log( MSG_QUERY, query );
    //Log( MSG_INFO, query );    
    pthread_mutex_lock( &SQLMutex );  
    while(Qfail)
    {    
        if(mysql_query( Mysql, query )!=0)
        {
            Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
            if(Reconnect( )==-1)
            {
                Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
                pthread_mutex_unlock( &SQLMutex );        
                return false;
            }
            else Qfail = false;
        }    
        else Qfail = false;
       // Log( MSG_INFO, "mysql answer: %s", mysql_error( Mysql ) );
    }
    result = mysql_store_result( Mysql );
    return result;
}

MYSQL_RES* CDatabase::QUse( char *Format, ...)
{
    bool Qfail = true;    
    char query[1024];
	va_list ap; 
    va_start( ap, Format );
	vsprintf( query, Format, ap ); 
	va_end  ( ap );  
    result = NULL;
    Log( MSG_QUERY, query );
    pthread_mutex_lock( &SQLMutex );  
    while(Qfail)
    {    
        if(mysql_query( Mysql, query )!=0)
        {
            Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
            if(Reconnect( )==-1)
            {
                Log( MSG_FATALERROR, "Could not execute query: %s", mysql_error( Mysql ) );   
                pthread_mutex_unlock( &SQLMutex );        
                return false;
            }
            else Qfail = false;
        }    
        else Qfail = false;
    }
    result = mysql_use_result( Mysql );
    return result;
}

bool CDatabase::DoSQL(char *Format, ...) {
    int retval;
	char Buffer[1024];
	va_list ap; va_start( ap, Format );
	vsprintf( Buffer, Format, ap ); 
	va_end  ( ap );
	retval = mysql_query( Mysql, Buffer );
	if( retval!=0 ) Log( MSG_ERROR, "MySQL Query Error '%s'", mysql_error( Mysql ) );
	return (retval==0);
}

void CDatabase::QFree( )
{
    mysql_free_result( result );
    pthread_mutex_unlock( &SQLMutex );
}

bool CDatabase::Ping( )
{
    if ( time( NULL ) > ( LastPing + Timeout ) ) {
      Log(MSG_INFO, "MySql Ping");
      LastPing = time( NULL );
      MYSQL_RES *res = QStore("SELECT @@wait_timeout");
      if (res == NULL) {
        Log( MSG_ERROR, "MySQL Ping Failed, Attempting reconnect" );
        QFree( );
        return !Reconnect( );
      }
      MYSQL_ROW row = mysql_fetch_row(res);
      Timeout = atoi(row[0]) - 60;
      QFree( );
      Log( MSG_INFO, "MySql Pong, new Timeout: %d seconds", Timeout);
    }
    return true;
}
