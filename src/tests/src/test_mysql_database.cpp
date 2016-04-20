#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include <string>
#include <exception>
#include <thread>

#include "config.h"
#include "cmysql_database.h"
#include "cmysql_databasepool.h"
#include "database.h"

TEST( TestMySQL_Database, TestConstructor )
{
	const ::configFile::Database	&dbb = Core::Config::getInstance("test.ini").database();
	std::string host = dbb.host();
	std::string _database = dbb.database();
	std::string user = dbb.user();
	std::string pass = dbb.password();

	EXPECT_NO_FATAL_FAILURE( Core::CMySQL_Database database(host.c_str(), _database.c_str(), user.c_str(), pass.c_str()));
}

TEST( TestMySQL_Database, TestQExecuteFails )
{
	Core::CMySQL_Database database;
	EXPECT_ANY_THROW(database.Connect("azertyu", "dhfui34hf4", "dfj3gqf27", "Rahf7823"));
        EXPECT_ANY_THROW(database.QExecute("DROP TABLE IF EXISTS test_table;"));
        EXPECT_ANY_THROW(database.QExecute("CREATE TABLE test_table(id INT);"));
        EXPECT_ANY_THROW(database.QExecute("DROP TABLE test_table;") );
        EXPECT_THROW(database.QExecute("use table test_table;"), std::exception);
}

TEST( TestMySQL_Database, TestQExecute )
{
        const ::configFile::Database    &dbb = Core::Config::getInstance("test.ini").database();
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
        const ::configFile::Database    &dbb = Core::Config::getInstance("test.ini").database();
        std::string host = dbb.host();
        std::string _database = dbb.database();
        std::string user = dbb.user();
        std::string pass = dbb.password();

	Core::CMySQL_Database	database(host.c_str(), _database.c_str(), user.c_str(), pass.c_str());
	database.QExecute("DROP TABLE IF EXISTS test_table;");
	database.QExecute("CREATE TABLE test_table(id INT, value INT, str VARCHAR(64), fl FLOAT, data BLOB);");
	database.QExecute("insert into test_table(id, value, str, fl, data) values(0, 12, 'plop', 3.14, '\x08\x12\x24');");
	database.QExecute("insert into test_table(id, value, str, fl, data) values(1, NULL, 'null values', NULL, NULL);");
        database.QExecute("DELETE FROM accounts WHERE username='test';");
        database.QExecute("INSERT INTO accounts (username, password, access) VALUES ('test', 'cc03e747a6afbbcbf8be7668acfebee5', 1);");
	std::unique_ptr<Core::IResult>	res;
	EXPECT_NO_FATAL_FAILURE(res = std::move(database.QStore("select * from test_table;")));
	uint32_t	id;
	EXPECT_EQ(res->getInt("id", id), true);
	EXPECT_EQ(id, 0);
	std::string	str;
	EXPECT_EQ(res->getString("str", str), true);
	EXPECT_EQ(str, "plop");
	float dec;
	EXPECT_EQ(res->getFloat("fl", dec), true);
	EXPECT_FLOAT_EQ(dec, 3.14);
	res->useRow(1);
	EXPECT_EQ(res->getInt("value", id), false);
	EXPECT_EQ(id, 0);
}

TEST(TestMySQL_Database, TestError)
{
        const ::configFile::Database    &dbb = Core::Config::getInstance("test.ini").database();
        std::string host = dbb.host();
        std::string _database = dbb.database();
        std::string user = dbb.user();
        std::string pass = dbb.password();

	Core::CMySQL_Database	database(host, _database, user, pass);
}

TEST(TestMySQL_Database, TestMultipleResults)
{
	const ::configFile::Database    &dbb = Core::Config::getInstance("test.ini").database();
	std::string host = dbb.host();
	std::string _database = dbb.database();
	std::string user = dbb.user();
	std::string pass = dbb.password();

	Core::CMySQL_Database	database(host.c_str(), _database.c_str(), user.c_str(), pass.c_str());
	database.QExecute("DROP TABLE IF EXISTS test_table;");
	database.QExecute("CREATE TABLE test_table(id INT, value INT, str VARCHAR(64), data BLOB);");
	database.QExecute("insert into test_table(id, value, str, data) values(0, 12, 'plop', '\x08\x12\x24');");
	database.QExecute("insert into test_table(id, value, str, data) values(1, NULL, 'null values', NULL);");
	std::unique_ptr<Core::IResult>	res, res2;
	EXPECT_NO_FATAL_FAILURE(res = std::move(database.QStore("select * from test_table;")));
	EXPECT_NO_FATAL_FAILURE(res2 = std::move(database.QStore("select * from test_table;")));
}

TEST(TestMySQL_Database, TestThreaded)
{
	const ::configFile::Database    &dbb = Core::Config::getInstance("test.ini").database();
	std::string host = dbb.host();
	std::string _database = dbb.database();
	std::string user = dbb.user();
	std::string pass = dbb.password();

	Core::CMySQL_Database	database(host.c_str(), _database.c_str(), user.c_str(), pass.c_str());
	database.QExecute("DROP TABLE IF EXISTS test_table;");
	database.QExecute("CREATE TABLE test_table(id INT, value INT, str VARCHAR(64), data BLOB);");
	database.QExecute("insert into test_table(id, value, str, data) values(0, 12, 'plop', '\x08\x12\x24');");
	database.QExecute("insert into test_table(id, value, str, data) values(1, NULL, 'null values', NULL);");
	
	std::thread first([&database] () {
			std::unique_ptr<Core::IResult> res;
			for (int i = 0; i < 20; ++i)
				EXPECT_NO_FATAL_FAILURE(res = std::move(database.QStore("select * from test_table;")));
			});
	std::thread second([&database] () {
			std::unique_ptr<Core::IResult> res;
			for (int i = 0; i < 20; ++i)
				EXPECT_NO_FATAL_FAILURE(res = std::move(database.QStore("select * from test_table;")));
			});
	first.join();
	second.join();
}

TEST(TestMySQL_Database, TestIRowAndIterator)
{
	const ::configFile::Database    &dbb = Core::Config::getInstance("test.ini").database();
	std::string host = dbb.host();
	std::string _database = dbb.database();
	std::string user = dbb.user();
	std::string pass = dbb.password();

	Core::CMySQL_Database	database(host, _database, user, pass);
	database.QExecute("DROP TABLE IF EXISTS test_table;");
	database.QExecute("CREATE TABLE test_table(id INT, value INT, str VARCHAR(64), data BLOB);");
	database.QExecute("insert into test_table(id, value, str, data) values(0, 12, 'plop', '\x08\x12\x24');");
	database.QExecute("insert into test_table(id, value, str, data) values(1, NULL, NULL, NULL);");

	std::unique_ptr<Core::IResult> res = database.QStore("select * from test_table;");
	EXPECT_NO_FATAL_FAILURE([&res] () {
			std::string a;
			for (auto &it : *res) {
				std::string tmp;
				uint32_t t;
				it->getInt("id", t);
				a += std::to_string(t);
				if (it->getString("str", tmp))
					a += tmp;
			}
			EXPECT_EQ(a, "0plop1");
		}());
}

TEST(TestMySQL_DatabasePool, TestGetInstance) {
	struct Filename {
		constexpr static const char *str() { return "test.ini"; }
	};
	EXPECT_NO_FATAL_FAILURE(Core::IDatabasePool &pool = Core::databasePoolFilename<Filename>::getInstance());
	EXPECT_NO_FATAL_FAILURE([] () {
			Core::IDatabasePool &pool = Core::databasePoolFilename<Filename>::getInstance();
			EXPECT_NO_FATAL_FAILURE(Core::IDatabase &database = pool.getDatabase());
		});

	EXPECT_NO_FATAL_FAILURE([] () {
			Core::IDatabasePool &pool = Core::databasePoolFilename<Filename>::getInstance();
			Core::IDatabase &database = pool.getDatabase();
			database.QExecute("DROP TABLE IF EXISTS test_table;");
			database.QExecute("CREATE TABLE test_table(id INT, value INT, str VARCHAR(64), data BLOB);");
			database.QExecute("insert into test_table(id, value, str, data) values(0, 12, 'plop', '\x08\x12\x24');");
			database.QExecute("insert into test_table(id, value, str, data) values(1, NULL, NULL, NULL);");
			auto res = database.QStore("select * from test_table;");
			std::string s;
			res->getString("str", s);
			EXPECT_EQ(s, "plop");
		}());
}

TEST(TestMySQL_Database, TestSQLEscape) {
	EXPECT_EQ(Core::CMySQL_Database::escapeData("test"), "test");
	EXPECT_EQ(Core::CMySQL_Database::escapeData("1test1"), "1test1");
	EXPECT_EQ(Core::CMySQL_Database::escapeData("\\test1"), "\\test1");
	EXPECT_EQ(Core::CMySQL_Database::escapeData("'test1"), "\\'test1");
	EXPECT_EQ(Core::CMySQL_Database::escapeData("\"test1"), "\\\"test1");
	EXPECT_EQ(Core::CMySQL_Database::escapeData("%test1"), "\\%test1");
	EXPECT_EQ(Core::CMySQL_Database::escapeData("_test1"), "\\_test1");
	EXPECT_EQ(Core::CMySQL_Database::escapeData(";test1"), "test1");
}
