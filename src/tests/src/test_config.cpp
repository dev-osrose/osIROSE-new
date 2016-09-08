#include "gtest/gtest.h"
#include "config.h"

TEST(SetupTesting, TestDefault) {
  EXPECT_NO_FATAL_FAILURE(Core::Config::getInstance("server.ini"));
}

TEST(Config, TestDefault) {
  EXPECT_NO_FATAL_FAILURE(Core::Config::getInstance("test.ini"));
}

TEST(Config, TestDatabase) {
  Core::Config &config = Core::Config::getInstance();
  const ::configFile::Database &dbb = config.database();
  EXPECT_EQ("localhost", dbb.host());
  EXPECT_EQ("osirose", dbb.database());
  EXPECT_EQ("root", dbb.user());
  EXPECT_EQ("", dbb.password());
  EXPECT_EQ(3306, dbb.port());
}

TEST(Config, TestServer) {
  Core::Config &config = Core::Config::getInstance();
  const ::configFile::Server &sd = config.serverdata();
  EXPECT_EQ(0, sd.id());
  EXPECT_EQ("127.0.0.1", sd.ip());
  EXPECT_EQ(1, sd.accesslevel());
  EXPECT_EQ(0, sd.parentid());
  EXPECT_EQ(0, sd.maxconnections());
  EXPECT_EQ(true, sd.usethreads());
  EXPECT_EQ(0, sd.mode());
}
