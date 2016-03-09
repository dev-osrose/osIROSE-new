#include "ccharserver.h"
#include "ccharisc.h"
#include "config.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  Core::CLogConsole::SetDisplayOmittable(false);
  Core::CLogConsole Logger("CharServer");
  Logger.icprintf("Starting up server...\n\n");

  Core::Config& config = Core::Config::getInstance();
  Core::NetworkThreadPool::GetInstance(config.serverdata().maxthreads());

  CCharServer clientServer;
  CCharServer iscServer(true);
  CCharISC iscClient;
  iscClient.Init(config.char_server().loginip(), config.char_server().loginiscport());
  iscClient.SetLogin(true);

  clientServer.Init(config.serverdata().ip(), config.char_server().clientport());
  clientServer.Listen();
  clientServer.GetISCList().push_front((CCharISC*)&iscClient);

  iscServer.Init(config.serverdata().ip(), config.char_server().iscport());
  iscServer.Listen();
  iscClient.Connect();

  while (clientServer.IsActive()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  Core::NetworkThreadPool::DeleteInstance();
  return 0;
}
