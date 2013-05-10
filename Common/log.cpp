#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #include <cstdio>
#endif
#include <cstdio>
#include "log.h"

// Basic colors
typedef enum
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
} COLORS;

// Our base colors
static int __BACKGROUND = BLACK;
static int __FOREGROUND = LIGHTGRAY;

// Change console text color
void textcolor(int color)
{
    #ifdef _WIN32
    __FOREGROUND = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color + (__BACKGROUND << 4));
    #else
    
    #endif
}

// This function logs based on flags
void Log( enum msg_type flag, char *Format, ... )
{
	va_list ap;	      // For arguments
	va_start( ap, Format );

	switch (flag) {
		case MSG_NONE: // direct printf replacement
			textcolor(WHITE);
			vprintf( Format, ap );
			break;
		case MSG_STATUS: 
			textcolor(GREEN);
			printf("[STATUS]: ");
			break;
		case MSG_SQL: 
			textcolor(CYAN);
			printf("[SQL]: ");
			break;
		case MSG_INFO: 
			textcolor(LIGHTGREEN);
			printf("[INFO]: ");
			break;
		case MSG_NOTICE: 
			textcolor(LIGHTCYAN);
			printf("[NOTICE]: ");
			break;
		case MSG_WARNING: 
			textcolor(YELLOW);
			printf("[WARNING]: ");
			break;
		case MSG_DEBUG: 
			textcolor(LIGHTBLUE); 
			printf("[DEBUG]: ");
			break;
		case MSG_ERROR:
			textcolor(RED);
			printf("[ERROR]: ");
			break;
		case MSG_FATALERROR:
			textcolor(LIGHTRED);
			printf("[FATAL ERROR]: ");
			break;
		case MSG_HACK:
			textcolor(LIGHTRED);
			printf("[HACK]: ");
			break;
		case MSG_LOAD:
			textcolor(BROWN);
			printf("[LOADING]: ");
			break;
		case MSG_GMACTION:
			textcolor(MAGENTA);
			printf("[GM ACTION]: ");
			break;	   
        case MSG_START:
            textcolor(MAGENTA);
            vprintf( Format, ap );
            printf( "\r\n" );
            break;                    								
	}
	textcolor(LIGHTGRAY);
	if(flag!=MSG_QUERY)
	{
    	vprintf( Format, ap );
    	printf( (flag==MSG_LOAD) ? "\r" : "\n" );
    }
	FILE *fh;
    switch(LOG_THISSERVER)
    {
        case LOG_LOGIN_SERVER:
            fh = fopen(LOG_DIRECTORY LOG_FILENAME_LOGINFILE, "a+" );     
        break;
        case LOG_CHARACTER_SERVER:
            fh = fopen(LOG_DIRECTORY LOG_FILENAME_CHARFILE, "a+" );                 
        break;
        case LOG_WORLD_SERVER:
            fh = fopen(LOG_DIRECTORY LOG_FILENAME_WORLDFILE, "a+" );                 
        break;
        case LOG_SAME_FILE:
            fh = fopen(LOG_DIRECTORY LOG_DEFAULT_FILE, "a+" );                 
        break;                                                
    }	    	
    if(flag==MSG_QUERY)
    {
        if(fh!=NULL)
            fclose(fh);
        fh = fopen(LOG_DIRECTORY "queries.txt", "a+" ); 
    }
	if ( fh != NULL ) 
    {
		vfprintf( fh, Format, ap );
		fputc( '\n', fh );	
		fclose( fh );
	}

	va_end  ( ap );
}
