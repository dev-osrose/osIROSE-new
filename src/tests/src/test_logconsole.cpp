#include "gtest/gtest.h"

#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include <thread>
#include "logconsole.h"

Core::CLogConsole g_Log( "TestConsoleLogging" );

TEST( TestConsoleLogging, TestIncprintf )
{
	g_Log.incprintf( CL_RESET CL_BLUE "I" CL_MAGENTA "t " CL_WHITE "W" CL_GRAY "o" CL_RED "r" CL_GREEN "k" CL_YELLOW "s" CL_CYAN "!\n" CL_RESET );
}

TEST( TestConsoleLogging, TestIcprintf )
{
	g_Log.icprintf( CL_RESET CL_BLUE "I" CL_MAGENTA "t " CL_WHITE "W" CL_GRAY "o" CL_RED "r" CL_GREEN "k" CL_YELLOW "s" CL_CYAN "!\n" CL_RESET );
}

TEST( TestConsoleLogging, TestOicprintf )
{
	g_Log.SetDisplayOmittable( true );
	g_Log.oicprintf( CL_RESET CL_BLUE "I" CL_MAGENTA "t " CL_WHITE "W" CL_GRAY "o" CL_RED "r" CL_GREEN "k" CL_YELLOW "s" CL_CYAN "!\n" CL_RESET );
	g_Log.SetDisplayOmittable( false );
}

TEST( TestConsoleLogging, TestEicprintf )
{
	g_Log.eicprintf( CL_RESET CL_BLUE "I" CL_MAGENTA "t " CL_WHITE "W" CL_GRAY "o" CL_RED "r" CL_GREEN "k" CL_YELLOW "s" CL_CYAN "!\n" CL_RESET );
}

TEST( TestConsoleLogging, TestDcprintf )
{
	g_Log.dcprintf( CL_RESET CL_BLUE "I" CL_MAGENTA "t " CL_WHITE "W" CL_GRAY "o" CL_RED "r" CL_GREEN "k" CL_YELLOW "s" CL_CYAN "!\n" CL_RESET );
}

TEST( TestConsoleLogging, TestRenderProgressBar )
{
	// Currently doesn't work as intended so disabled for now.
	for ( int i = 0; i <= 100; i++ )
	{
		g_Log.RenderProgressBar( 60, i );
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) ); //sim work to be done
	}
	g_Log.dcprintf("\n");
}
