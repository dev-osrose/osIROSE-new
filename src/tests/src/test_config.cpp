#include "gtest/gtest.h"
#include "config.h"

TEST(SetupTesting, TestDefault) {
  EXPECT_NO_FATAL_FAILURE(Core::Config::getInstance("server.json"));
}

TEST(Config, TestDefault) {
  EXPECT_NO_FATAL_FAILURE(Core::Config::getInstance("test.json"));
}

TEST(Config, TestDatabase) {
  Core::Config &config = Core::Config::getInstance();
  auto dbb = config.database();
  EXPECT_EQ("127.0.0.1", dbb.host);
  EXPECT_EQ("osirose", dbb.database);
  EXPECT_EQ("root", dbb.user);
  EXPECT_EQ("", dbb.password);
  EXPECT_EQ(3306, dbb.port);
}

TEST(Config, TestServer) {
  Core::Config &config = Core::Config::getInstance();
  auto &sd = config.serverData();
  EXPECT_EQ(0, sd.id);
  EXPECT_EQ("127.0.0.1", sd.ip);
  EXPECT_EQ(1, sd.accessLevel);
  EXPECT_EQ(0, sd.parentId);
  EXPECT_EQ(0, sd.maxConnections);
  EXPECT_EQ(true, sd.useThreads);
  EXPECT_EQ(0, sd.mode);
}
