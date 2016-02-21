
#include "cmapserver.h"
#include "cmapisc.h"

int main( int argc, char* argv[] )
{
	(void)argc;
	(void)argv;
	CLogConsole::SetDisplayOmittable( false );
	CLogConsole Logger( "Map Server" );
	Logger.icprintf( "Starting up server...\n\n" );

	CMapServer clientServer;
	CMapServer iscServer( true );
	CMapISC    iscClient;
	iscClient.Init( "127.0.0.1", 29110 );
	iscClient.SetChar( true );

	clientServer.Init( "127.0.0.1", 29200 );
	clientServer.Listen( );
	clientServer.GetISCList( ).push_front( (CMapISC*)&iscClient );

	iscServer.Init( "127.0.0.1", 29210 );
	iscServer.Listen( );
	iscClient.Connect( );

	while ( clientServer.IsActive( ) )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}

	return 0;
}
