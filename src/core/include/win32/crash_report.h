#pragma once

#ifdef ENABLE_CRASH_REPORTS
  #ifdef BREAKPAD
    #include <client/windows/handler/exception_handler.h>
  #endif
#endif

namespace Core {
  class CrashReport : public ICrashReport {
  public:
    CrashReport(std::string path, [[maybe_unused]] std::string app_name, [[maybe_unused]] std::string pipe = "");
    void set_url([[maybe_unused]] std::string url);

  private:
#ifdef ENABLE_CRASH_REPORTS
  #ifdef BREAKPAD
    google_breakpad::ExceptionHandler _exception_handler;
  #endif
#endif
  };
}