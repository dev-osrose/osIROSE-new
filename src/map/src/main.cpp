
#include "cmapserver.h"
#include "cmapisc.h"
#include "config.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  Core::CLogConsole::SetDisplayOmittable(false);
  Core::CLogConsole Logger("Map Server");
  Logger.icprintf("Starting up server...\n\n");

  Config& config = Config::getInstance();
  Core::NetworkThreadPool::GetInstance(config.serverdata().maxthreads());

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
