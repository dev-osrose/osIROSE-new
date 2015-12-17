#include "gtest/gtest.h"
#include <string>
#include <exception>

#include "cmysql_database.h"

TEST( TestMySQL_Database, TestConstructor )
{
	EXPECT_NO_FATAL_FAILURE( CMySQL_Database database("127.0.0.1", "osirose", "root", "root"));
}

TEST( TestMySQL_Database, TestQExecute )
{
    CMySQL_Database database("127.0.0.1", "osirose", "root", "root");
	EXPECT_NO_FATAL_FAILURE( database.QExecute("DROP TABLE IF EXISTS test_table;"));
	EXPECT_NO_FATAL_FAILURE(database.QExecute("CREATE TABLE test_table(id INT);"));
	EXPECT_NO_FATAL_FAILURE(database.QExecute("DROP TABLE test_table;") );
	EXPECT_THROW(database.QExecute("use table test_table;"), std::exception);
}

TEST(TestMySQL_Database, TestQStore)
{
	CMySQL_Database	database("127.0.0.1", "osirose", "root", "root");
	database.QExecute("DROP TABLE IF EXISTS test_table;");
	database.QExecute("CREATE TABLE test_table(id INT, value INT, str VARCHAR(64), data BLOB);");
	database.QExecute("insert into test_table(id, value, str, data) values(0, 12, 'plop', '\x08\x12\x24');");
	database.QExecute("insert into test_table(id, value, str, data) values(1, NULL, 'null values', NULL);");
	IResult	*res;
	EXPECT_NO_FATAL_FAILURE(res = database.QStore("select * from test_table;"));
	uint32_t	id;
	EXPECT_EQ(res->getInt("id", id), true);
	EXPECT_EQ(id, 0);
	std::string	str;
	EXPECT_EQ(res->getString("str", str), true);
	EXPECT_EQ(str, "plop");
	res->useRow(1);
	EXPECT_EQ(res->getInt("value", id), false);
	EXPECT_EQ(id, 0);
	EXPECT_NO_FATAL_FAILURE(delete res);
}

TEST(TestMySQL_Database, TestError)
{
	CMySQL_Database	database("127.0.0.1", "osirose", "root", "root");
}
