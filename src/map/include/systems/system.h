#pragma once

#include "entityComponents.h"
#include "systemmanager.h"
#include "packets.h"
#include "logconsole.h"

namespace Systems
{
  class System {
  public:
    System(SystemManager& manager) : manager_( manager ),
                                     logger_( Core::CLog::GetLogger( Core::log_type::SYSTEM ).lock() ) {}

    virtual ~System() = default;

    virtual void update(EntityManager&, double dt) = 0;

  protected:
    SystemManager& manager_;
    std::shared_ptr<spdlog::logger> logger_;
  };
}
