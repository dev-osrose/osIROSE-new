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

#ifndef _WIN32
  #include "unix/crash_report.h"
#elif _WIN32
  //#include "win32/crash_report.h"
#endif