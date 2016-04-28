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

#include "cloginserver.h"
#include "config.h"
#include "logconsole.h"
#include "version.h"

namespace {
void DisplayTitle()
{
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
  {
    log->notice( "--------------------------------" );
    log->notice( "        osIROSE 2 Alpha         " );
    log->notice( "  http://forum.dev-osrose.com/  " );
    log->notice( "--------------------------------" );
    log->notice( "Git Branch/Revision: {}/{}", GIT_BRANCH, GIT_COMMIT_HASH );
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

  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
    log->notice( "Starting up server..." );

  Core::Config& config = Core::Config::getInstance();
  Core::CLog::SetLevel((spdlog::level::level_enum)config.login_server().log_level());
  DisplayTitle();
  CheckUser();
  
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
