#pragma once

#include <string>

namespace Core {
  class ICrashReport {
  public:
    virtual ~ICrashReport() {};
    
    // This forces the application to crash.
    static void crash() { volatile int* a = (int*)(nullptr); *a = 1; }
  };
}

#ifdef _WIN32
  #include "win32/crash_report.h"
#else
  #include "unix/crash_report.h"
#endif