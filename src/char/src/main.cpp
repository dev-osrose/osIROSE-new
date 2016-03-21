#include "ccharserver.h"
#include "ccharisc.h"
#include "config.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);

  if(auto log = console.lock())
    log->notice("Starting up server...");

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
  if(auto log = console.lock())
    log->notice( "Server shutting down..." );
  Core::NetworkThreadPool::DeleteInstance();
  spdlog::drop_all();
  return 0;
}
