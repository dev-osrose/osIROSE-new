#include "cloginserver.h"


int main( int argc, char* argv[] )
{
	(void)argc;
	(void)argv;
	CLogConsole::SetDisplayOmittable(false);
	//setvbuf (stdout, NULL, _IONBF, 0);

	CLogConsole Logger("LoginServer");
	Logger.icprintf("Starting up server...\n\n");

	CLoginServer clientServer;
	CLoginServer iscServer( true );

	clientServer.Init( "127.0.0.1", 29000 );
	clientServer.Listen( );

	iscServer.Init( "127.0.0.1", 29010 );
	iscServer.Listen( );

	while( clientServer.IsActive() )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
	return 0;
}
