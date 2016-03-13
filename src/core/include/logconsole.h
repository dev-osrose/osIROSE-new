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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>  // atexit

#ifndef _WIN32
//#include <wchar.h>
#endif
namespace Core {

// console colour codes (ctrl-c ctrl-v from eAthena)
#define CL_RESET "\033[0m"
//#define CL_HIDDEN		"\033[8m"
#define CL_CLS "\033[2J"
#define CL_CLL "\033[K"

// font settings
#define CL_BOLD "\033[1m"
#define CL_NORM CL_RESET
#define CL_NORMAL CL_RESET
#define CL_NONE CL_RESET
// foreground color and bold font (bright color on windows)
#define CL_WHITE "\033[1;37m"
#define CL_GRAY "\033[1;30m"
#define CL_RED "\033[1;31m"
#define CL_GREEN "\033[1;32m"
#define CL_YELLOW "\033[1;33m"
#define CL_BLUE "\033[1;34m"
#define CL_MAGENTA "\033[1;35m"
#define CL_CYAN "\033[1;36m"

// background color
#define CL_BG_BLACK "\033[40m"
#define CL_BG_RED "\033[41m"
#define CL_BG_GREEN "\033[42m"
#define CL_BG_YELLOW "\033[43m"
#define CL_BG_BLUE "\033[44m"
#define CL_BG_MAGENTA "\033[45m"
#define CL_BG_CYAN "\033[46m"
#define CL_BG_WHITE "\033[47m"
// foreground color and normal font (normal color on windows)
#define CL_LT_BLACK "\033[0;30m"
#define CL_LT_RED "\033[0;31m"
#define CL_LT_GREEN "\033[0;32m"
#define CL_LT_YELLOW "\033[0;33m"
#define CL_LT_BLUE "\033[0;34m"
#define CL_LT_MAGENTA "\033[0;35m"
#define CL_LT_CYAN "\033[0;36m"
#define CL_LT_WHITE "\033[0;37m"
// foreground color and bold font (bright color on windows)
#define CL_BT_BLACK "\033[1;30m"
#define CL_BT_RED "\033[1;31m"
#define CL_BT_GREEN "\033[1;32m"
#define CL_BT_YELLOW "\033[1;33m"
#define CL_BT_BLUE "\033[1;34m"
#define CL_BT_MAGENTA "\033[1;35m"
#define CL_BT_CYAN "\033[1;36m"
#define CL_BT_WHITE "\033[1;37m"

/*!
 * \class CLogConsole
 *
 * \brief The console logging class
 *
 * This class is used to log on screen
 *
 * \author RavenX8
 * \date march 2016
 */
class CLogConsole {
 private:
  char ident[64];

  static int maxlen;
  static bool display_omittable;
  int mylen;

 public:
  /*!
   * \param[in] The identification that will apear before the log
   */
  CLogConsole(const char* id);
  /*!
   * \brief The logging won't print the identification but will correctly indent the message
   *
   * \param[in] fmt The printf's style formated string
   * \param[in] ... The other arguments as needed
   * \param[out] int The number of characters written to the screen
   */
  int incprintf(const char* fmt, ...);  // indentated coloured printf -> no
                                        // identification, but appropriate
                                        // indent from the left
  /*!
   * \brief The logging will print the identification and will correctly indent the message
   *
   * \param[in] fmt The printf's style formated string
   * \param[in] ... The other arguments as needed
   * \param[out] int The number of characters written to the screen
   */
  int icprintf(const char* fmt,
               ...);  // identified coloured printf -> [ident] sdfhsfh

  /*!
   * \brief The logging will print the identification and will correctly indent the message only if \s display_omittable is set
   * \sa SetDisplayOmittable
   *
   * \param[in] fmt The printf's style formated string
   * \param[in] ... The other arguments as needed
   * \param[out] int The number of characters written to the screen
   */
  int oicprintf(const char* fmt, ...);  // omittable identified coloured printf
                                        // -> [ident] sdfhsfh if debugging

  /*!
   * \brief The logging will print the identification and will correctly indent the error message (flashier colors)
   *
   * \param[in] fmt The printf's style formated string
   * \param[in] ... The other arguments as needed
   * \param[out] int The number of characters written to the screen
   */
  int eicprintf(const char* fmt, ...);  // identified coloured error printf ->
                                        // [ident] sdfhsfh, but with flashier
                                        // colours

  /*!
   * \brief The logging won't print the identification and won't indent the message
   *
   * \param[in] fmt The printf's style formated string
   * \param[in] ... The other arguments as needed
   * \param[out] int The number of characters written to the screen
   */
  int dcprintf(const char* fmt, ...);   // direct coloured printf -> sdfhsh

  /*!
   * \brief A progress bar will be shown on screen
   *
   * \param[in] width The total width of the progress bar
   * \param[in] per The current percentage of the progress bar
   */
  void RenderProgressBar(int width, int per);

  /*!
   * \brief Change the logger identity code after the instance was created
   *
   * \param[in] id The identification string
   */
  void SetIdentity(const char* id);

  /*!
   * \brief Change if all debugging messages are shown from now on or not
   *
   * \param[in] a Set if debug messages will be printed or not
   */
  static void SetDisplayOmittable(bool a) { display_omittable = a; }
};

// void console_mutex_init();
}

#endif
