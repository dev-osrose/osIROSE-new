
#include "cmapserver.h"
#include "cmapisc.h"
#include "config.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  spdlog::set_pattern( "[%H:%M:%S.%e %z] [%L] [thread %t] %v" );
  size_t q_size = 1048576;
  spdlog::set_async_mode( q_size );
  auto console = spdlog::stdout_logger_mt( "console" );
  console->notice( "Starting up server..." );

  Core::Config& config = Core::Config::getInstance();
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

  console->notice( "Server shutting down..." );
  Core::NetworkThreadPool::DeleteInstance();
  spdlog::drop_all();
  return 0;
}
