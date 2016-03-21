/*!
 * \file logconsole.h
 *
 * \author RavenX8
 * \date march 2016
 *
 * Console logging class
 */
#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include <ios>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>

namespace Core {

namespace Color {
enum CodeFG {
  // Reset formats
  CL_RESET = 0,
  CL_BOLD = 21,
  CL_DIM = 22,
  CL_UNDERLINE = 24,
  CL_BLINK = 25,
  CL_REVERSE = 26,
  CL_HIDDEN = 27,

  // Formatting
  FG_BOLD = 1,
  FG_DIM = 2,
  FG_UNDERLINE = 3,
  FG_BLINK = 4,
  FG_REVERSE = 5,
  FG_HIDDEN = 6,

  // Coloring
  FG_GRAY = 30,
  FG_RED = 31,
  FG_GREEN = 32,
  FG_BLUE = 34,
  FG_MAGENTA = 35,
  FG_CYAN = 36,
  FG_LT_GRAY = 37,
  FG_DK_GRAY = 90,
  FG_LT_RED = 91,
  FG_LT_GREEN = 92,
  FG_LT_YELLOW = 93,
  FG_LT_BLUE = 94,
  FG_LT_MAGENTA = 95,
  FG_LT_CYAN = 96,
  FG_WHITE = 97,
  FG_DEFAULT = 39
};

enum CodeBG {
  BG_BLACK = 40,
  BG_RED = 41,
  BG_GREEN = 42,
  BG_YELLOW = 43,
  BG_BLUE = 44,
  BG_MAGENTA = 45,
  BG_CYAN = 46,
  BG_LT_GRAY = 47,
  BG_DK_GRAY = 100,
  BG_LT_RED = 101,
  BG_LT_GREEN = 102,
  BG_LT_YELLOW = 103,
  BG_LT_BLUE = 104,
  BG_LT_MAGENTA = 105,
  BG_LT_CYAN = 106,
  BG_LT_WHITE = 107,

  BG_DEFAULT = 49
};
  std::ostream& operator<<(std::ostream& os, Core::Color::CodeFG code);
  std::ostream& operator<<(std::ostream& os, Core::Color::CodeBG code);
}

enum class log_type {
  GENERAL,
  NETWORK,
  DATABASE
};

class CLog {
public:
  static std::shared_ptr<spdlog::logger> CreateLoggers(log_type _type, spdlog::level::level_enum _level = spdlog::level::notice);
};
}

#endif
