#include "gtest/gtest.h"
#include "config.h"

TEST(Config, TestDefault)
{
	EXPECT_NO_FATAL_FAILURE(Config::getInstance("test.ini"));
}

TEST(Config, TestDatabase)
{
	Config	&config = Config::getInstance();
	const ::configFile::Database	&dbb = config.database();
	EXPECT_EQ("localhost", dbb.host());
	EXPECT_EQ("osirose", dbb.database());
	EXPECT_EQ("root", dbb.user());
	EXPECT_EQ("root", dbb.password());
	EXPECT_EQ(3306, dbb.port());
}

TEST(Config, TestServer)
{
	Config	&config = Config::getInstance();
	const ::configFile::Server	&sd = config.serverdata();
	EXPECT_EQ(0, sd.id());
	EXPECT_EQ("127.0.0.1", sd.ip());
	EXPECT_EQ(100, sd.accesslevel());
	EXPECT_EQ(0, sd.parentid());
	EXPECT_EQ(100, sd.maxconnections());
	EXPECT_EQ(false, sd.usethreads());
	EXPECT_EQ(0, sd.mode());
}
