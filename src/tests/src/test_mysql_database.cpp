#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include <string>
#include <exception>

#include "config.h"
#include "cmysql_database.h"

TEST( TestMySQL_Database, TestConstructor )
{
	const ::configFile::Database	&dbb = Config::getInstance("test.ini").database();
	std::string host = dbb.host();
	std::string _database = dbb.database();
	std::string user = dbb.user();
	std::string pass = dbb.password();

	EXPECT_NO_FATAL_FAILURE( Core::CMySQL_Database database(host.c_str(), _database.c_str(), user.c_str(), pass.c_str()));
}

TEST( TestMySQL_Database, TestQExecuteFails )
{
	Core::CMySQL_Database database;
	EXPECT_ANY_THROW(database.Connect("fdafsdohuohy530", "dhfui34hf4", "dfj3gqf27", "Rahf7823"));
        EXPECT_ANY_THROW(database.QExecute("DROP TABLE IF EXISTS test_table;"));
        EXPECT_ANY_THROW(database.QExecute("CREATE TABLE test_table(id INT);"));
        EXPECT_ANY_THROW(database.QExecute("DROP TABLE test_table;") );
        EXPECT_THROW(database.QExecute("use table test_table;"), std::exception);
}

TEST( TestMySQL_Database, TestQExecute )
{
        const ::configFile::Database    &dbb = Config::getInstance("test.ini").database();
        std::string host = dbb.host();
        std::string _database = dbb.database();
        std::string user = dbb.user();
        std::string pass = dbb.password();

	Core::CMySQL_Database database(host.c_str(), _database.c_str(), user.c_str(), pass.c_str());
	EXPECT_NO_FATAL_FAILURE( database.QExecute("DROP TABLE IF EXISTS test_table;"));
	EXPECT_NO_FATAL_FAILURE(database.QExecute("CREATE TABLE test_table(id INT);"));
	EXPECT_NO_FATAL_FAILURE(database.QExecute("DROP TABLE test_table;") );
	EXPECT_THROW(database.QExecute("use table test_table;"), std::exception);
}

TEST(TestMySQL_Database, TestQStore)
{
        const ::configFile::Database    &dbb = Config::getInstance("test.ini").database();
        std::string host = dbb.host();
        std::string _database = dbb.database();
        std::string user = dbb.user();
        std::string pass = dbb.password();

	Core::CMySQL_Database	database(host.c_str(), _database.c_str(), user.c_str(), pass.c_str());
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
        const ::configFile::Database    &dbb = Config::getInstance("test.ini").database();
        std::string host = dbb.host();
        std::string _database = dbb.database();
        std::string user = dbb.user();
        std::string pass = dbb.password();

	Core::CMySQL_Database	database(host, _database, user, pass);
}
