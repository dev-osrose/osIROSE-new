#include "cloginserver.h"
#include "config.h"
#include "cmysql_database.h"
#include "cdatabase_manager.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  Core::CLogConsole::SetDisplayOmittable(false);
  // setvbuf (stdout, NULL, _IONBF, 0);

  Core::CLogConsole Logger("LoginServer");
  Logger.icprintf("Starting up server...\n\n");

  Core::Config& config = Core::Config::getInstance();
  Core::NetworkThreadPool::GetInstance(config.serverdata().maxthreads());
  const ::configFile::Database& dbb = config.database();

  std::string host = dbb.host();
  std::string database = dbb.database();
  std::string user = dbb.user();
  std::string pass = dbb.password();

  CDatabaseMgr::getInstance().GetDatabase().Connect( host, database, user, pass );

  CLoginServer clientServer;
  CLoginServer iscServer(true);

  clientServer.Init(config.serverdata().ip(), config.login_server().clientport());
  clientServer.Listen();

  iscServer.Init(config.serverdata().ip(), config.login_server().iscport());
  iscServer.Listen();

  while (clientServer.IsActive()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  Core::NetworkThreadPool::DeleteInstance();
  return 0;
}
