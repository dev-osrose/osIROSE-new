#include "gtest/gtest.h"

#include "cmysql_database.h"

TEST( TestMySQL_Database, TestConstructor )
{
	EXPECT_NO_FATAL_FAILURE( CMySQL_Database database("127.0.0.1", "osirose", "root", "root"));
}

TEST( TestMySQL_Database, TestConnect )
{
	CMySQL_Database database("127.0.0.1", "osirose", "root", "root");
	EXPECT_TRUE( database.Connect() );
}

TEST( TestMySQL_Database, TestDisconnect )
{
        CMySQL_Database database("127.0.0.1", "osirose", "root", "root");
        EXPECT_TRUE( database.Connect() );
	EXPECT_NO_FATAL_FAILURE( database.Disconnect() );
}

TEST( TestMySQL_Database, TestQExecute )
{
        CMySQL_Database database("127.0.0.1", "osirose", "root", "root");
        EXPECT_TRUE( database.Connect() );
	EXPECT_TRUE( database.QExecute("DROP TABLE IF EXISTS test_table; CREATE TABLE test_table(id INT); DROP TABLE test_table") );
        EXPECT_NO_FATAL_FAILURE( database.Disconnect() );
}
