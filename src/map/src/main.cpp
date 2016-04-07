
#include "cmapserver.h"
#include "cmapisc.h"
#include "config.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
    log->notice( "Starting up server..." );

  Core::Config& config = Core::Config::getInstance();
  Core::CLog::SetLevel((spdlog::level::level_enum)config.map_server().log_level());
  if(auto log = console.lock()) {
    log->set_level((spdlog::level::level_enum)config.map_server().log_level());
    log->trace("Trace logs are enabled.");
    log->debug("Debug logs are enabled.");
    log->info("Info logs are enabled.");
  }
  Core::NetworkThreadPool::GetInstance(config.serverdata().maxthreads());

  CMapServer clientServer;
  CMapServer iscServer(true);
  CMapISC* iscClient = new CMapISC();
  iscClient->Init(config.map_server().charip(), config.map_server().chariscport());
  iscClient->SetChar(true);

  clientServer.Init(config.serverdata().ip(), config.map_server().clientport());
  clientServer.Listen();
  clientServer.GetISCList().push_front(iscClient);

  iscServer.Init(config.serverdata().ip(), config.map_server().iscport());
  iscServer.Listen();
  iscClient->Connect();

  while (clientServer.IsActive()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  if(auto log = console.lock())
    log->notice( "Server shutting down..." );
  Core::NetworkThreadPool::DeleteInstance();
  spdlog::drop_all();
  return 0;
}
