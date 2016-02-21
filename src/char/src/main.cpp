#include "ccharserver.h"
#include "ccharisc.h"

int main( int argc, char* argv[] )
{
	(void)argc;
	(void)argv;
	CLogConsole::SetDisplayOmittable( false );
	CLogConsole Logger( "CharServer" );
	Logger.icprintf( "Starting up server...\n\n" );

	CCharServer clientServer;
	CCharServer iscServer( true );
	CCharISC    iscClient;
	iscClient.Init( "127.0.0.1", 29010 );
	iscClient.SetLogin( true );

	clientServer.Init( "127.0.0.1", 29100 );
	clientServer.Listen();
	clientServer.GetISCList().push_front( (CCharISC*)&iscClient );

	iscServer.Init( "127.0.0.1", 29110 );
	iscServer.Listen();
	iscClient.Connect();

	while (clientServer.IsActive())
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
	return 0;
}
