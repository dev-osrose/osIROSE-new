#include "cloginserver.h"
#include "config.h"
#include "logconsole.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
    log->notice( "Starting up server..." );

  Core::Config& config = Core::Config::getInstance();
  Core::CLog::SetLevel((spdlog::level::level_enum)config.login_server().log_level());
  if(auto log = console.lock()) {
    log->set_level((spdlog::level::level_enum)config.login_server().log_level());
    log->trace("Trace logs are enabled.");
    log->debug("Debug logs are enabled.");
    log->info("Info logs are enabled.");
  }

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

  if(auto log = console.lock())
    log->notice( "Server shutting down..." );
  Core::NetworkThreadPool::DeleteInstance();
  spdlog::drop_all();
  return 0;
}
