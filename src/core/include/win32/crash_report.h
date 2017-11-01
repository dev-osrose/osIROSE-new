#pragma once

//#include <client/windows/handler/exception_handler.h>

namespace Core {
  class CrashReport : ICrashReport {
  public:
    CrashReport(std::string path, std::string pipe = "") {};
    
  private:
    //google_breakpad::ExceptionHandler _exception_handler;
  };
}