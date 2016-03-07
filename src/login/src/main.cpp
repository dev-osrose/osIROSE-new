#include "cloginserver.h"
#include "config.h"
#include "cmysql_database.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  Core::CLogConsole::SetDisplayOmittable(false);
  // setvbuf (stdout, NULL, _IONBF, 0);

  Core::CLogConsole Logger("LoginServer");
  Logger.icprintf("Starting up server...\n\n");
  Core::NetworkThreadPool::GetInstance();

  Core::Config& config = Core::Config::getInstance();
  const ::configFile::Database& dbb = config.database();

  std::string host = dbb.host();
  std::string _database = dbb.database();
  std::string user = dbb.user();
  std::string pass = dbb.password();

  Core::CMySQL_Database database(host, _database, user, pass);

  CLoginServer clientServer;
  CLoginServer iscServer(true);

  clientServer.Init("127.0.0.1", 29000);
  clientServer.Listen();

  iscServer.Init("127.0.0.1", 29010);
  iscServer.Listen();

  while (clientServer.IsActive()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  Core::NetworkThreadPool::DeleteInstance();
  return 0;
}
