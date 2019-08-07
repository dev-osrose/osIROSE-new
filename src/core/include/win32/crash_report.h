#pragma once

#ifdef ENABLE_CRASH_REPORTS
  #include <client/windows/handler/exception_handler.h>
#endif

namespace Core {
  class CrashReport : public ICrashReport {
  public:
    CrashReport(std::string path, [[maybe_unused]] std::string app_name, [[maybe_unused]] std::string pipe = "");
    void set_url([[maybe_unused]] std::string url);

  private:
#ifdef ENABLE_CRASH_REPORTS
    google_breakpad::ExceptionHandler _exception_handler;
#endif
  };
}