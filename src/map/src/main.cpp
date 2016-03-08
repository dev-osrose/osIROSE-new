
#include "cmapserver.h"
#include "cmapisc.h"
#include "config.h"
#include "cdatabase_manager.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  Core::CLogConsole::SetDisplayOmittable(false);
  Core::CLogConsole Logger("Map Server");
  Logger.icprintf("Starting up server...\n\n");

  Core::Config& config = Core::Config::getInstance();
  Core::NetworkThreadPool::GetInstance(config.serverdata().maxthreads());

  const ::configFile::Database& dbb = config.database();

  std::string host = dbb.host();
  std::string database = dbb.database();
  std::string user = dbb.user();
  std::string pass = dbb.password();

  CDatabaseMgr::getInstance().GetDatabase().Connect( host, database, user, pass );

  CMapServer clientServer;
  CMapServer iscServer(true);
  CMapISC iscClient;
  iscClient.Init(config.map_server().charip(), config.map_server().chariscport());
  iscClient.SetChar(true);

  clientServer.Init(config.serverdata().ip(), config.map_server().clientport());
  clientServer.Listen();
  clientServer.GetISCList().push_front((CMapISC*)&iscClient);

  iscServer.Init(config.serverdata().ip(), config.map_server().iscport());
  iscServer.Listen();
  iscClient.Connect();

  while (clientServer.IsActive()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  Core::NetworkThreadPool::DeleteInstance();

  return 0;
}
