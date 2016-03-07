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

  Core::Config& config = Core::Config::getInstance();
  Core::NetworkThreadPool::GetInstance(config.serverdata().maxthreads());

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
