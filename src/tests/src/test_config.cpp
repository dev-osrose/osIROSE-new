#include "gtest/gtest.h"
#include "config.h"

TEST(Config, TestDefault)
{
	{
		EXPECT_NO_FATAL_FAILURE(Config config("test.ini"));
	}
	EXPECT_NO_FATAL_FAILURE(Config config("test.ini"));
}

TEST(Config, TestDatabase)
{
	Config	config("test.ini");
	const ::configFile::Database	&dbb = config.database();
	EXPECT_NE("localhost", dbb.host());
	EXPECT_NE("osirose", dbb.database());
	EXPECT_NE("root", dbb.user());
	EXPECT_NE("root", dbb.password());
	EXPECT_NE(3306, dbb.port());
}

TEST(Config, TestServer)
{
	Config	config("test.ini");
	const ::configFile::ServerData	&sd = config.serverdata();
	EXPECT_NE(0, sd.id());
	EXPECT_NE("127.0.0.1", sd.ip());
	EXPECT_NE(100, sd.accesslevel());
	EXPECT_NE(0, sd.parentid());
	EXPECT_NE(100, sd.maxconnections());
	EXPECT_NE(false, sd.usethreads());
	EXPECT_NE(0, sd.mode());
}
