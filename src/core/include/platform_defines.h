#ifndef _PLATFORM_DEFINES_H_
#define _PLATFORM_DEFINES_H_

namespace Core {

//#if !defined( WIN32 ) && !defined( _WIN32 )
//  #include <sys/time.h>
  #include <chrono>

  namespace Time {
    inline std::chrono::steady_clock::time_point GetTickCount()
    {
      return std::chrono::steady_clock::now();
    }
  } // end namespace Time
//  #endif
} // end namespace Core
#endif
