/*******************************************************************************************
      ___  ___  ___ ___ 
  ___| _ \/ _ \/ __| __|
 / -_)   / (_) \__ \ _| 
 \___|_|_\\___/|___/___|

 eROSE Server Develped by Brett19.
 hROSE Team: Brett19 and bdaan
 Year: 2006
 Release Date: Summer-Winter 2006
*******************************************************************************************/
#ifndef __ROSE_CONFIG_FUNCTIONS__
#define __ROSE_CONFIG_FUNCTIONS__

// -----------------------------------------------------------------------------------------
// Config load functions
// -----------------------------------------------------------------------------------------
char *ConfigGetString( char *pcFile, char *pcName, char *pcDefault );
unsigned ConfigGetInt( char *pcFile, char *pcName, unsigned uDefault );

// -----------------------------------------------------------------------------------------

#endif

