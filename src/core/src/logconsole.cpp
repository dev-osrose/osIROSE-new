#undef UNICODE

#include <mutex>
#include "logconsole.h"
#include <stdint.h>
namespace Core {
#define SBUF_SIZE \
  2048  // never put less that what's required for the debug message

#define NEWBUF(buf)               \
  struct {                        \
    char s_[SBUF_SIZE];           \
    struct StringBuf* d_;         \
    char* v_;                     \
    int l_;                       \
  } buf = {"", NULL, NULL, 0}; \ \
//define NEWBUF

#define BUFVPRINTF(buf, fmt, args)                            \
  buf.l_ = vsnprintf(buf.s_, SBUF_SIZE, fmt, args);           \
  if (buf.l_ >= 0 && buf.l_ < SBUF_SIZE) {/* static buffer */ \
    buf.v_ = buf.s_;                                          \
  }
// define BUFVPRINTF

#define BUFVAL(buf) buf.v_
#define BUFLEN(buf) buf.l_

#define FREEBUF(buf) \
  buf.v_ = NULL; \  \
//define FREEBUF

#define ISDIGIT(d) (d >= '0' && d <= '9')

#ifdef _WIN32

// pthread_mutex_t console_mutex=PTHREAD_MUTEX_INITIALIZER;
std::mutex console_mutex;

#include <windows.h>
// XXX adapted from eApp (comments are left untouched) [flaviojs]

///////////////////////////////////////////////////////////////////////////////
//  ansi compatible printf with control sequence parser for windows
//  fast hack, handle with care, not everything implemented
//
// \033[#;...;#m - Set Graphics Rendition (SGR)
//
//  printf("\x1b[1;31;40m");	// Bright red on black
//  printf("\x1b[3;33;45m");	// Blinking yellow on magenta (blink not
//  implemented)
//  printf("\x1b[1;30;47m");	// Bright black (grey) on dim white
//
//  Style           Foreground      Background
//  1st Digit       2nd Digit       3rd Digit		RGB
//  0 - Reset       30 - Black      40 - Black		000
//  1 - FG Bright   31 - Red        41 - Red		100
//  2 - Unknown     32 - Green      42 - Green		010
//  3 - Blink       33 - Yellow     43 - Yellow		110
//  4 - Underline   34 - Blue       44 - Blue		001
//  5 - BG Bright   35 - Magenta    45 - Magenta	101
//  6 - Unknown     36 - Cyan       46 - Cyan		011
//  7 - Reverse     37 - White      47 - White		111
//  8 - Concealed (invisible)
//
// \033[#A - Cursor Up (CUU)
//    Moves the cursor up by the specified number of lines without changing
//    columns.
//    If the cursor is already on the top line, this sequence is ignored. \e[A
//    is equivalent to \e[1A.
//
// \033[#B - Cursor Down (CUD)
//    Moves the cursor down by the specified number of lines without changing
//    columns.
//    If the cursor is already on the bottom line, this sequence is ignored.
//    \e[B is equivalent to \e[1B.
//
// \033[#C - Cursor Forward (CUF)
//    Moves the cursor forward by the specified number of columns without
//    changing lines.
//    If the cursor is already in the rightmost column, this sequence is
//    ignored. \e[C is equivalent to \e[1C.
//
// \033[#D - Cursor Backward (CUB)
//    Moves the cursor back by the specified number of columns without changing
//    lines.
//    If the cursor is already in the leftmost column, this sequence is ignored.
//    \e[D is equivalent to \e[1D.
//
// \033[#E - Cursor Next Line (CNL)
//    Moves the cursor down the indicated # of rows, to column 1. \e[E is
//    equivalent to \e[1E.
//
// \033[#F - Cursor Preceding Line (CPL)
//    Moves the cursor up the indicated # of rows, to column 1. \e[F is
//    equivalent to \e[1F.
//
// \033[#G - Cursor Horizontal Absolute (CHA)
//    Moves the cursor to indicated column in current row. \e[G is equivalent to
//    \e[1G.
//
// \033[#;#H - Cursor Position (CUP)
//    Moves the cursor to the specified position. The first # specifies the line
//    number,
//    the second # specifies the column. If you do not specify a position, the
//    cursor moves to the home position:
//    the upper-left corner of the screen (line 1, column 1).
//
// \033[#;#f - Horizontal & Vertical Position
//    (same as \033[#;#H)
//
// \033[s - Save Cursor Position (SCP)
//    The current cursor position is saved.
//
// \033[u - Restore cursor position (RCP)
//    Restores the cursor position saved with the (SCP) sequence \033[s.
//    (addition, restore to 0,0 if nothinh was saved before)
//

// \033[#J - Erase Display (ED)
//    Clears the screen and moves to the home position
//    \033[0J - Clears the screen from cursor to end of display. The cursor
//    position is unchanged. (default)
//    \033[1J - Clears the screen from start to cursor. The cursor position is
//    unchanged.
//    \033[2J - Clears the screen and moves the cursor to the home position
//    (line 1, column 1).
//
// \033[#K - Erase Line (EL)
//    Clears the current line from the cursor position
//    \033[0K - Clears all characters from the cursor position to the end of the
//    line (including the character at the cursor position). The cursor position
//    is unchanged. (default)
//    \033[1K - Clears all characters from start of line to the cursor position.
//    (including the character at the cursor position). The cursor position is
//    unchanged.
//    \033[2K - Clears all characters of the whole line. The cursor position is
//    unchanged.

/*
not implemented

\033[#L
IL: Insert Lines: The cursor line and all lines below it move down # lines,
leaving blank space. The cursor position is unchanged. The bottommost # lines
are lost. \e[L is equivalent to \e[1L.
\033[#M
DL: Delete Line: The block of # lines at and below the cursor are deleted; all
lines below them move up # lines to fill in the gap, leaving # blank lines at
the bottom of the screen. The cursor position is unchanged. \e[M is equivalent
to \e[1M.
\033[#\@
ICH: Insert CHaracter: The cursor character and all characters to the right of
it move right # columns, leaving behind blank space. The cursor position is
unchanged. The rightmost # characters on the line are lost. \e[\@ is equivalent
to \e[1\@.
\033[#P
DCH: Delete CHaracter: The block of # characters at and to the right of the
cursor are deleted; all characters to the right of it move left # columns,
leaving behind blank space. The cursor position is unchanged. \e[P is equivalent
to \e[1P.

Escape sequences for Select Character Set
*/

#define is_console(handle) (FILE_TYPE_CHAR == GetFileType(handle))

///////////////////////////////////////////////////////////////////////////////
static int VFPRINTF(HANDLE handle, const char* fmt, va_list argptr) {
  /////////////////////////////////////////////////////////////////
  /* XXX Two streams are being used. Disabled to avoid inconsistency [flaviojs]
  static COORD saveposition = {0,0};
  */

  /////////////////////////////////////////////////////////////////
  unsigned long written;
  char* p, *q;
  NEWBUF(tempbuf);  // temporary buffer

  if (!fmt || !*fmt) return 0;

  // Print everything to the buffer
  BUFVPRINTF(tempbuf, fmt, argptr);

  // pthread_mutex_lock(&console_mutex);
  console_mutex.lock();

  // start with processing
  p = BUFVAL(tempbuf);
  while ((q = strchr(p, 0x1b)) != NULL) {  // find the escape character
    if (0 ==
        WriteConsole(handle, p, q - p, &written, 0))  // write up to the escape
      WriteFile(handle, p, q - p, &written, 0);

    if (q[1] != '[') {  // write the escape char (whatever purpose it has)
      if (0 == WriteConsole(handle, q, 1, &written, 0))
        WriteFile(handle, q, 1, &written, 0);
      p = q + 1;  // and start searching again
    } else {      // from here, we will skip the '\033['
      // we break at the first unprocessible position
      // assuming regular text is starting there
      uint8_t numbers[16], numpoint = 0;
      CONSOLE_SCREEN_BUFFER_INFO info;

      // initialize
      GetConsoleScreenBufferInfo(handle, &info);
      memset(numbers, 0, sizeof(numbers));

      // skip escape and bracket
      q = q + 2;
      while (1) {
        if (ISDIGIT(*q)) {  // add number to number array, only accept 2digits,
                            // shift out the rest
          // so // \033[123456789m will become \033[89m
          numbers[numpoint] = (numbers[numpoint] << 4) | (*q - '0');
          ++q;
          // and next character
          continue;
        } else if (*q == ';') {  // delimiter
          if (numpoint < sizeof(numbers) /
                             sizeof(*numbers)) {  // go to next array position
            numpoint++;
          } else {  // array is full, so we 'forget' the first value
            memmove(numbers, numbers + 1,
                    sizeof(numbers) / sizeof(*numbers) - 1);
            numbers[sizeof(numbers) / sizeof(*numbers) - 1] = 0;
          }
          ++q;
          // and next number
          continue;
        } else if (*q == 'm') {  // \033[#;...;#m - Set Graphics Rendition (SGR)
          uint8_t i;
          for (i = 0; i <= numpoint; ++i) {
            if (0x00 == (0xF0 & numbers[i])) {  // upper nibble 0
              if (0 == numbers[i]) {            // reset
                info.wAttributes =
                    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
              } else if (1 == numbers[i]) {  // set foreground intensity
                info.wAttributes |= FOREGROUND_INTENSITY;
              } else if (5 == numbers[i]) {  // set background intensity
                info.wAttributes |= BACKGROUND_INTENSITY;
              } else if (7 == numbers[i]) {  // reverse colors (just xor them)
                info.wAttributes ^= FOREGROUND_RED | FOREGROUND_GREEN |
                                    FOREGROUND_BLUE | BACKGROUND_RED |
                                    BACKGROUND_GREEN | BACKGROUND_BLUE;
              }
              // case '2': // not existing
              // case '3':	// blinking (not implemented)
              // case '4':	// unterline (not implemented)
              // case '6': // not existing
              // case '8': // concealed (not implemented)
              // case '9': // not existing
            } else if (0x20 == (0xF0 & numbers[i])) {  // off

              if (1 == numbers[i]) {  // set foreground intensity off
                info.wAttributes &= ~FOREGROUND_INTENSITY;
              } else if (5 == numbers[i]) {  // set background intensity off
                info.wAttributes &= ~BACKGROUND_INTENSITY;
              } else if (7 == numbers[i]) {  // reverse colors (just xor them)
                info.wAttributes ^= FOREGROUND_RED | FOREGROUND_GREEN |
                                    FOREGROUND_BLUE | BACKGROUND_RED |
                                    BACKGROUND_GREEN | BACKGROUND_BLUE;
              }
            } else if (0x30 == (0xF0 & numbers[i])) {  // foreground
              uint8_t num = numbers[i] & 0x0F;
              if (num == 9) info.wAttributes |= FOREGROUND_INTENSITY;
              if (num > 7) num = 7;  // set white for 37, 38 and 39
              info.wAttributes &=
                  ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
              if ((num & 0x01) > 0)  // lowest bit set = red
                info.wAttributes |= FOREGROUND_RED;
              if ((num & 0x02) > 0)  // second bit set = green
                info.wAttributes |= FOREGROUND_GREEN;
              if ((num & 0x04) > 0)  // third bit set = blue
                info.wAttributes |= FOREGROUND_BLUE;
            } else if (0x40 == (0xF0 & numbers[i])) {  // background
              uint8_t num = numbers[i] & 0x0F;
              if (num == 9) info.wAttributes |= BACKGROUND_INTENSITY;
              if (num > 7) num = 7;  // set white for 47, 48 and 49
              info.wAttributes &=
                  ~(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
              if ((num & 0x01) > 0)  // lowest bit set = red
                info.wAttributes |= BACKGROUND_RED;
              if ((num & 0x02) > 0)  // second bit set = green
                info.wAttributes |= BACKGROUND_GREEN;
              if ((num & 0x04) > 0)  // third bit set = blue
                info.wAttributes |= BACKGROUND_BLUE;
            }
          }
          // set the attributes
          SetConsoleTextAttribute(handle, info.wAttributes);
        } else if (*q == 'J') {  // \033[#J - Erase Display (ED)
          //    \033[0J - Clears the screen from cursor to end of display. The
          //    cursor position is unchanged.
          //    \033[1J - Clears the screen from start to cursor. The cursor
          //    position is unchanged.
          //    \033[2J - Clears the screen and moves the cursor to the home
          //    position (line 1, column 1).
          uint8_t num =
              (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F);
          int cnt;
          uint32_t tmp;
          COORD origin = {0, 0};
          if (num == 1) {  // chars from start up to and including cursor
            cnt = info.dwSize.X * info.dwCursorPosition.Y +
                  info.dwCursorPosition.X + 1;
          } else if (num == 2) {  // Number of chars on screen.
            cnt = info.dwSize.X * info.dwSize.Y;
            SetConsoleCursorPosition(handle, origin);
          } else  // 0 and default
          {       // number of chars from cursor to end
            origin = info.dwCursorPosition;
            cnt = info.dwSize.X * (info.dwSize.Y - info.dwCursorPosition.Y) -
                  info.dwCursorPosition.X;
          }
          FillConsoleOutputAttribute(handle, info.wAttributes, cnt, origin,
                                     &tmp);
          FillConsoleOutputCharacter(handle, ' ', cnt, origin, &tmp);
        } else if (*q == 'K') {  // \033[K  : clear line from actual position to
                                 // end of the line
          //    \033[0K - Clears all characters from the cursor position to the
          //    end of the line.
          //    \033[1K - Clears all characters from start of line to the cursor
          //    position.
          //    \033[2K - Clears all characters of the whole line.

          uint8_t num =
              (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F);
          COORD origin = {0, info.dwCursorPosition.Y};
          SHORT cnt;
          uint32_t tmp;
          if (num == 1) {
            cnt = info.dwCursorPosition.X + 1;
          } else if (num == 2) {
            cnt = info.dwSize.X;
          } else  // 0 and default
          {
            origin = info.dwCursorPosition;
            cnt =
                info.dwSize.X -
                info.dwCursorPosition.X;  // how many spaces until line is full
          }
          FillConsoleOutputAttribute(handle, info.wAttributes, cnt, origin,
                                     &tmp);
          FillConsoleOutputCharacter(handle, ' ', cnt, origin, &tmp);
        } else if (*q == 'H' ||
                   *q == 'f') {  // \033[#;#H - Cursor Position (CUP)
          // \033[#;#f - Horizontal & Vertical Position
          // The first # specifies the line number, the second # specifies the
          // column.
          // The default for both is 1
          info.dwCursorPosition.X = (numbers[numpoint])
                                        ? (numbers[numpoint] >> 4) * 10 +
                                              ((numbers[numpoint] & 0x0F) - 1)
                                        : 0;
          info.dwCursorPosition.Y =
              (numpoint && numbers[numpoint - 1])
                  ? (numbers[numpoint - 1] >> 4) * 10 +
                        ((numbers[numpoint - 1] & 0x0F) - 1)
                  : 0;

          if (info.dwCursorPosition.X >= info.dwSize.X)
            info.dwCursorPosition.Y = info.dwSize.X - 1;
          if (info.dwCursorPosition.Y >= info.dwSize.Y)
            info.dwCursorPosition.Y = info.dwSize.Y - 1;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 's') {  // \033[s - Save Cursor Position (SCP)
          /* XXX Two streams are being used. Disabled to avoid inconsistency
          [flaviojs]
          CONSOLE_SCREEN_BUFFER_INFO info;
          GetConsoleScreenBufferInfo(handle, &info);
          saveposition = info.dwCursorPosition;
          */
        } else if (*q == 'u') {  // \033[u - Restore cursor position (RCP)
          /* XXX Two streams are being used. Disabled to avoid inconsistency
          [flaviojs]
          SetConsoleCursorPosition(handle, saveposition);
          */
        } else if (*q == 'A') {  // \033[#A - Cursor Up (CUU)
          // Moves the cursor UP # number of lines
          info.dwCursorPosition.Y -=
              (numbers[numpoint])
                  ? (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F)
                  : 1;

          if (info.dwCursorPosition.Y < 0) info.dwCursorPosition.Y = 0;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 'B') {  // \033[#B - Cursor Down (CUD)
          // Moves the cursor DOWN # number of lines
          info.dwCursorPosition.Y +=
              (numbers[numpoint])
                  ? (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F)
                  : 1;

          if (info.dwCursorPosition.Y >= info.dwSize.Y)
            info.dwCursorPosition.Y = info.dwSize.Y - 1;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 'C') {  // \033[#C - Cursor Forward (CUF)
          // Moves the cursor RIGHT # number of columns
          info.dwCursorPosition.X +=
              (numbers[numpoint])
                  ? (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F)
                  : 1;

          if (info.dwCursorPosition.X >= info.dwSize.X)
            info.dwCursorPosition.X = info.dwSize.X - 1;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 'D') {  // \033[#D - Cursor Backward (CUB)
          // Moves the cursor LEFT # number of columns
          info.dwCursorPosition.X -=
              (numbers[numpoint])
                  ? (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F)
                  : 1;

          if (info.dwCursorPosition.X < 0) info.dwCursorPosition.X = 0;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 'E') {  // \033[#E - Cursor Next Line (CNL)
          // Moves the cursor down the indicated # of rows, to column 1
          info.dwCursorPosition.Y +=
              (numbers[numpoint])
                  ? (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F)
                  : 1;
          info.dwCursorPosition.X = 0;

          if (info.dwCursorPosition.Y >= info.dwSize.Y)
            info.dwCursorPosition.Y = info.dwSize.Y - 1;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 'F') {  // \033[#F - Cursor Preceding Line (CPL)
          // Moves the cursor up the indicated # of rows, to column 1.
          info.dwCursorPosition.Y -=
              (numbers[numpoint])
                  ? (numbers[numpoint] >> 4) * 10 + (numbers[numpoint] & 0x0F)
                  : 1;
          info.dwCursorPosition.X = 0;

          if (info.dwCursorPosition.Y < 0) info.dwCursorPosition.Y = 0;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 'G') {  // \033[#G - Cursor Horizontal Absolute (CHA)
          // Moves the cursor to indicated column in current row.
          info.dwCursorPosition.X = (numbers[numpoint])
                                        ? (numbers[numpoint] >> 4) * 10 +
                                              ((numbers[numpoint] & 0x0F) - 1)
                                        : 0;

          if (info.dwCursorPosition.X >= info.dwSize.X)
            info.dwCursorPosition.X = info.dwSize.X - 1;
          SetConsoleCursorPosition(handle, info.dwCursorPosition);
        } else if (*q == 'L' || *q == 'M' || *q == '@' ||
                   *q == 'P') {  // not implemented, just skip
        } else {                 // no number nor valid sequencer
          // something is fishy, we break and give the current char free
          --q;
        }
        // skip the sequencer and search again
        p = q + 1;
        break;
      }  // end while
    }
  }
  if (*p)  // write the rest of the buffer
    if (0 == WriteConsole(handle, p, strlen(p), &written, 0))
      WriteFile(handle, p, strlen(p), &written, 0);

  console_mutex.unlock();
  // pthread_mutex_unlock(&console_mutex);
  FREEBUF(tempbuf);
  return 0;
}

static int FPRINTF(HANDLE handle, const char* fmt, ...) {
  int ret;
  va_list argptr;
  va_start(argptr, fmt);
  ret = VFPRINTF(handle, fmt, argptr);
  va_end(argptr);
  return ret;
}

#define FFLUSH(handle)

#define STDOUT GetStdHandle(STD_OUTPUT_HANDLE)
#define STDERR GetStdHandle(STD_ERROR_HANDLE)

#else  // not _WIN32

//#define VPRINTF	vprintf
//#define PRINTF	printf

#define is_console(file) (0 != isatty(fileno(file)))

// vprintf_without_ansiformats
/*int	VFPRINTF(FILE *file, const char *fmt, va_list argptr)
{
        char *p, *q;
        NEWBUF(tempbuf); // temporary buffer

        if(!fmt || !*fmt)
                return 0;

        // Print everything to the buffer
        BUFVPRINTF(tempbuf,fmt,argptr);

        // start with processing
        p = BUFVAL(tempbuf);
        while ((q = strchr(p, 0x1b)) != NULL)
        {	// find the escape character
                fprintf(file, "%.*s", (int)(q-p), p); // write up to the escape
                if( q[1]!='[' )
                {	// write the escape char (whatever purpose it has)
                        fprintf(file, "%.*s", 1, q);
                        p=q+1;	//and start searching again
                }
                else
                {	// from here, we will skip the '\033['
                        // we break at the first unprocessible position
                        // assuming regular text is starting there

                        // skip escape and bracket
                        q=q+2;
                        while(1)
                        {
                                if( ISDIGIT(*q) )
                                {
                                        ++q;
                                        // and next character
                                        continue;
                                }
                                else if( *q == ';' )
                                {	// delimiter
                                        ++q;
                                        // and next number
                                        continue;
                                }
                                else if( *q == 'm' )
                                {	// \033[#;...;#m - Set Graphics
Rendition (SGR)
                                        // set the attributes
                                }
                                else if( *q=='J' )
                                {	// \033[#J - Erase Display (ED)
                                }
                                else if( *q=='K' )
                                {	// \033[K  : clear line from actual
position to end of the line
                                }
                                else if( *q == 'H' || *q == 'f' )
                                {	// \033[#;#H - Cursor Position (CUP)
                                        // \033[#;#f - Horizontal & Vertical
Position
                                }
                                else if( *q=='s' )
                                {	// \033[s - Save Cursor Position (SCP)
                                }
                                else if( *q=='u' )
                                {	// \033[u - Restore cursor position
(RCP)
                                }
                                else if( *q == 'A' )
                                {	// \033[#A - Cursor Up (CUU)
                                        // Moves the cursor UP # number of lines
                                }
                                else if( *q == 'B' )
                                {	// \033[#B - Cursor Down (CUD)
                                        // Moves the cursor DOWN # number of
lines
                                }
                                else if( *q == 'C' )
                                {	// \033[#C - Cursor Forward (CUF)
                                        // Moves the cursor RIGHT # number of
columns
                                }
                                else if( *q == 'D' )
                                {	// \033[#D - Cursor Backward (CUB)
                                        // Moves the cursor LEFT # number of
columns
                                }
                                else if( *q == 'E' )
                                {	// \033[#E - Cursor Next Line (CNL)
                                        // Moves the cursor down the indicated #
of rows, to column 1
                                }
                                else if( *q == 'F' )
                                {	// \033[#F - Cursor Preceding Line (CPL)
                                        // Moves the cursor up the indicated #
of rows, to column 1.
                                }
                                else if( *q == 'G' )
                                {	// \033[#G - Cursor Horizontal Absolute
(CHA)
                                        // Moves the cursor to indicated column
in current row.
                                }
                                else if( *q == 'L' || *q == 'M' || *q == '@' ||
*q == 'P')
                                {	// not implemented, just skip
                                }
                                else
                                {	// no number nor valid sequencer
                                        // something is fishy, we break and give
the current char free
                                        --q;
                                }
                                // skip the sequencer and search again
                                p = q+1;
                                break;
                        }// end while
                }
        }
        if (*p)	// write the rest of the buffer
                fprintf(file, "%s", p);
        FREEBUF(tempbuf);
        return 0;
}
int	FPRINTF(FILE *file, const char *fmt, ...)
{
        int ret;
        va_list argptr;
        va_start(argptr, fmt);
        ret = VFPRINTF(file,fmt,argptr);
        va_end(argptr);
        return ret;
}*/

#define FFLUSH fflush

#define STDOUT stdout
#define STDERR stderr

#endif  // not _WIN32

static int clprintf(const char* fmt, ...) {
  int ret;
  va_list argptr;
  va_start(argptr, fmt);
#ifdef _WIN32
  ret = VFPRINTF(STDOUT, fmt, argptr);
#else
  ret = vprintf(fmt, argptr);
#endif
  va_end(argptr);
  return ret;
}

static int clvprintf(const char* fmt, va_list argptr) {
  int ret;
#ifdef _WIN32
  ret = VFPRINTF(STDOUT, fmt, argptr);
#else
  ret = vprintf(fmt, argptr);
#endif
  return ret;
}

CLogConsole::CLogConsole(const char* id) {
  mylen = strlen(id);
  if (mylen > maxlen) maxlen = mylen;
  strcpy(ident, id);
}

int CLogConsole::incprintf(const char* fmt, ...) {
  int ret;
  char buf[1024], buf2[64];

  memset(buf2, 0, 64);
  memset(buf2, ' ', maxlen + 3);

  va_list argptr;
  va_start(argptr, fmt);
  sprintf(buf, "%s%s", buf2, fmt);
  ret = clvprintf(buf, argptr);
  va_end(argptr);
  return ret;
}

int CLogConsole::icprintf(const char* fmt, ...) {
  int ret;
  char buf[1024], buf2[64];
  memset(buf2, 0, 64);
  memset(buf2, ' ', maxlen - mylen + 1);

  va_list argptr;
  va_start(argptr, fmt);
  sprintf(buf, CL_BT_GREEN "[" CL_LT_GREEN "%s" CL_BT_GREEN "]" CL_RESET "%s%s",
          ident, buf2, fmt);
  ret = clvprintf(buf, argptr);
  va_end(argptr);
  return ret;
}

int CLogConsole::oicprintf(const char* fmt, ...) {
  if (display_omittable) {
    int ret;
    char buf[1024], buf2[64];
    memset(buf2, 0, 64);
    memset(buf2, ' ', maxlen - mylen + 1);

    va_list argptr;
    va_start(argptr, fmt);

    sprintf(buf,
            CL_BT_GREEN "[" CL_LT_GREEN "%s" CL_BT_GREEN "]" CL_RESET "%s%s",
            ident, buf2, fmt);
    ret = clvprintf(buf, argptr);

    va_end(argptr);
    return ret;
  } else
    return 0;
}

int CLogConsole::eicprintf(const char* fmt, ...) {
  int ret;
  char buf[1024], buf2[64];
  memset(buf2, 0, 64);
  memset(buf2, ' ', maxlen - mylen + 1);

  va_list argptr;
  va_start(argptr, fmt);

  sprintf(buf, CL_LT_RED "[" CL_BT_RED "%s" CL_LT_RED "]" CL_RESET "%s%s",
          ident, buf2, fmt);
  ret = clvprintf(buf, argptr);

  va_end(argptr);
  return ret;
}

int CLogConsole::dcprintf(const char* fmt, ...) {
  int ret;
  va_list argptr;
  va_start(argptr, fmt);

  ret = clvprintf(fmt, argptr);

  va_end(argptr);
  return ret;
}

void CLogConsole::RenderProgressBar(int width, int per) {
  int fwidth = (per * width) / 100;
  char b[80];
  wchar_t b2[80];
  memset(b, 0, 80);

#ifdef _WIN32
  memset(b, ' ', width - fwidth);
  memset(b2, 0, 160);
  for (int i = 0; i < fwidth; ++i) ((char*)b2)[i] = 0xDB;

  clprintf("\033[G\033[J");
  clprintf(CL_WHITE "[" CL_BG_GREEN "" CL_BT_GREEN);
  printf("%s", b2);

  clprintf(CL_BG_GREEN "%s" CL_RESET, b);
  clprintf(CL_RESET CL_WHITE "]" CL_RESET);
#else
  memset(b, '-', width - fwidth);
  memset(b2, 0, 160);
  for (int i = 0; i < fwidth; ++i) ((char*)b2)[i] = ' ';

  int complete = width * (float)per / 100;

  clprintf("\033[F\033[J");
  clprintf(CL_WHITE "[%3d%%] [" CL_BG_GREEN "" CL_BT_GREEN, per);

  for (int x = 0; x < complete; x++) clprintf(" ");
  for (int x = complete; x < width; x++)
    clprintf(
        "\033[0;102m"
        " " CL_RESET);
  // wprintf( L"%s", b2 );

  // clprintf( CL_RESET "" CL_BT_GREEN "%s" CL_RESET, b );
  clprintf(CL_RESET CL_WHITE "]\n" CL_RESET);
// clprintf( "\033[G\033[J" );
#endif
}

void CLogConsole::SetIdentity(const char* id) {
  mylen = strlen(id);
  if (mylen > maxlen) maxlen = mylen;
  strcpy(ident, id);
}

int CLogConsole::maxlen = 0;
bool CLogConsole::display_omittable = false;
}
