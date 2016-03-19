// #undef UNICODE
// 
#include "logconsole.h"

std::ostream& operator<<(std::ostream& os, Core::Color::Code code)
{
  return os << "\033[" << static_cast<int>(code) << "m";
}