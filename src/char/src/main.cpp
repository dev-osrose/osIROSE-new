// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ccharserver.h"
#include "ccharisc.h"
#include "config.h"
#include "version.h"
#include "network_thread_pool.h"
#include "cnetwork_asio.h"

namespace {
void DisplayTitle()
{
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
  {
    log->info( "--------------------------------" );
    log->info( "        osIROSE 2 Alpha         " );
    log->info( "  http://forum.dev-osrose.com/  " );
    log->info( "--------------------------------" );
    log->info( "Git Branch/Revision: {}/{}", GIT_BRANCH, GIT_COMMIT_HASH );
  }
}

void CheckUser()
{
#ifndef _WIN32
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
  {
    if ((getuid() == 0) && (getgid() == 0)) {
      log->warn( "You are running as the root superuser." );
      log->warn( "It is unnecessary and unsafe to run with root privileges." );
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
#endif
}
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  try {
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);

  if(auto log = console.lock())
    log->info("Starting up server...");

  Core::Config& config = Core::Config::getInstance();
  Core::CLog::SetLevel(static_cast<spdlog::level::level_enum>(config.char_server().log_level()));
  DisplayTitle();
  CheckUser();

  if(auto log = console.lock()) {
    log->set_level(static_cast<spdlog::level::level_enum>(config.char_server().log_level()));
    log->trace("Trace logs are enabled.");
    log->debug("Debug logs are enabled.");
  }
  Core::NetworkThreadPool::GetInstance(config.serverdata().maxthreads());

  CCharServer clientServer;
  CCharServer iscServer(true);
  CCharISC* iscClient = new CCharISC(new Core::CNetwork_Asio());
  iscClient->init(config.char_server().loginip(), config.char_server().loginiscport());
  iscClient->SetLogin(true);
  iscClient->connect();

  clientServer.init(config.serverdata().ip(), config.char_server().clientport());
  clientServer.listen();
  clientServer.GetISCList().push_front(std::unique_ptr<RoseCommon::CRoseClient>(iscClient));

  iscServer.init(config.serverdata().isclistenip(), config.char_server().iscport());
  iscServer.listen();

  while (clientServer.is_active()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  if(auto log = console.lock())
    log->info( "Server shutting down..." );
  Core::NetworkThreadPool::DeleteInstance();
  spdlog::drop_all();
  }
  catch (const spdlog::spdlog_ex& ex) {
     std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return 0;
}
