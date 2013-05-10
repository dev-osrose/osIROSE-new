//Made by Caali

#define __CROSEFILE_HPP__

/* Includes */
#include <stdio.h>
#include <stdlib.h>

#ifndef __CARRAY_HPP__
#include "CRoseArray.hpp"
#endif

/* MS Visual C++ Deprecation control defines */
#ifdef _MSC_VER
#	ifdef _CRT_SECURE_NO_DEPRECATE
#		define CFILE_NODEPRECATE
#	endif
#else
#	define CFILE_NODEPRECATE
#endif

//Debug:
//#define CFILE_NOUNICODE

/* Various defines */
#define CFILE_MAXLINELENGHT 255
#define CFILE_UNICODE_HEADER 0xFEFF

/* Variable type declarations */
/* ETC */
typedef bool					CFILE_BOOLEAN;
typedef size_t					CFILE_SIZE;

/* Unsigned */
typedef unsigned long long int	CFILE_ULONGLONG;
typedef unsigned long int		CFILE_ULONG;
typedef unsigned int			CFILE_UINT;
typedef unsigned short int		CFILE_USHORT;
typedef unsigned char			CFILE_UCHAR;

/* Signed */
typedef signed long long int	CFILE_SLONGLONG;
typedef signed long int			CFILE_SLONG;
typedef signed int				CFILE_SINT;
typedef signed short int		CFILE_SSHORT;
typedef signed char				CFILE_SCHAR;
typedef char					CFILE_CHAR;

/* File types */
typedef enum
{
	FT_NONE,
	FT_BINARY,
	FT_ASCII,
	FT_UNICODE
} FILETYPE;
/* File open modes */
typedef enum
{
	FM_NONE					= 0,
	FM_READ					= (1<<0),
	FM_WRITE				= (1<<1),
	FM_CREATEIFDOESNTEXIST	= (1<<2),
	FM_BINARY				= (1<<3),
	FM_UNICODE				= (1<<4),
	FM_READWRITE			= (FM_READ | FM_WRITE),
	FM_RW					= (FM_READWRITE | FM_CREATEIFDOESNTEXIST)
} FILEMODE;

/* File errors */
typedef enum
{
	FE_SUCCESS = 0,						//No error
	FE_EOF,								//CFile::EndOfFile( ) == true
	FE_NOFILEOPENED,					//fh == NULL
	FE_NORIGHTSFORACTION,				//Like when trying to read when only opening in write mode
	FE_INTERNALERROR,					//CFile-internal errors, supposed to destruct the CFile now!
	FE_UNICODENOTALLOWED,				//#ifdef CFILE_NOUNICODE and file is unicode
	FE_UNABLETOOPENFILE					//fopen( ) == NULL
} FILEERROR;

/* The CFile class */
class CRoseFile
{
	private:
        /** The current filetype
        */
		FILETYPE ftype;
        /** The last error occured
        */
		FILEERROR flasterr;
        /** The current file open mode bitfield
        */
		CFILE_CHAR fmode;
        /** Function to throw an error
        @param
		err: A FILEERROR
        */
		void DoError( FILEERROR err )
		{
			flasterr = err;
		}
        /** Converts the bitfield file open mode to c standart ASCII open modes
        @param
		mode: A byte with the bitfield
		@result
		string with the ASCII string
        */
		CFILE_CHAR*	ConvertBitFieldFileModeToASCIIFileMode( CFILE_SCHAR mode )
		{
			CFILE_CHAR* result = new CFILE_CHAR[30];
			CFILE_UCHAR len = 0;
			for( CFILE_CHAR i = 0; i<30; i++ ) result[i] = 0x00;
			if( (mode & FM_READWRITE) == FM_READWRITE )
			{
				if( (mode & FM_CREATEIFDOESNTEXIST) == FM_CREATEIFDOESNTEXIST )
				{
					result[0] = 'w';
					result[1] = '+';
					len = 2;
				}
				else
				{
					result[0] = 'r';
					result[1] = '+';
					len = 2;
				}
			}
			else if( (mode & FM_READ) == FM_READ )
			{
				result[0] = 'r';
				len = 1;
			}
			else if( (mode & FM_WRITE) == FM_WRITE )
			{
				result[0] = 'w';
				len = 1;
			}
			else
			{
				DoError( FE_INTERNALERROR );
				delete[] result;
				return NULL;
			}
			if( (mode & FM_BINARY) == FM_BINARY )
			{
				result[len] = 'b';
				len++;
			}
			if( (mode & FM_UNICODE) == FM_UNICODE )
			{
				printf( (CFILE_CHAR*)result, 30, "%s, ccs=UNICODE", (CFILE_CHAR*)result );
				len += 13;
			}
			result[len] = 0x00;
			return result;
		}

	public:
           
           /** The current filehandle
        */
		FILE* fh;
        /** Contructor
        */
		CRoseFile( )
		{
			fh = NULL;
			ftype = FT_NONE;
			fmode = FM_NONE;	
			flasterr = FE_SUCCESS;
		}
        /** Contructor opening a file after contructing
        @param
		fname: A string with the filename
		@param
		mode: A bitfield using the FILETYPE enumerator to specify the fopen mode
        */
		CRoseFile( CFILE_CHAR* fname, CFILE_CHAR mode )
		{
			fh = NULL;
			ftype = FT_NONE;
			fmode = FM_NONE;	
			flasterr = FE_SUCCESS;
			Open( fname, mode );
		}

		//Destructors
		~CRoseFile( )
		{
			if( fh != NULL ) Close( );
		}

        /** Opens a file
        @param
		fname: A string with the filename
		@param
		mode: A bitfield using the FILETYPE enumerator to specify the fopen mode
		@result
		Boolean indicating if Open successed or failed
		@note
		If there is already an opened file, CFile will close it and open the new file
        */
		CFILE_BOOLEAN Open( CFILE_CHAR* fname, CFILE_CHAR mode )
		{	
			if( fh != NULL ) Close( );
			fmode = mode;
			CFILE_CHAR* fasciimode = ConvertBitFieldFileModeToASCIIFileMode( fmode );
			if( fasciimode == NULL )
			{
				DoError( FE_INTERNALERROR );
				return false;
			}
			if( (fmode & FM_BINARY) == FM_BINARY )
			{
				ftype = FT_BINARY;
			}
		#ifndef CFILE_NODEPRECATE
			if( fopen_s( &fh, fname, fasciimode ) == 0 )
			{
		#else 
			if( ( fh = fopen( fname, fasciimode ) ) != NULL )
			{		
		#endif
				delete[] fasciimode;
				//Check if unicode or ascii
				if( ftype != FT_BINARY )
				{
					CFILE_USHORT buf;
					Read( &buf, sizeof( buf ), 1 );
					if( buf == CFILE_UNICODE_HEADER )
					{
						#ifndef CFILE_NOUNICODE
							fmode |= FM_UNICODE;
							Close( );
							CFILE_CHAR* fasciimode = ConvertBitFieldFileModeToASCIIFileMode( fmode );
							if( fasciimode == NULL )
							{
								DoError( FE_INTERNALERROR );
								return false;
							}
							#ifndef CFILE_NODEPRECATE
								fopen_s( &fh, fname, fasciimode );
							#else 
								fh = fopen( fname, fasciimode );
							#endif
							SetAbsPos( 2 );
							ftype = FT_UNICODE;
						#else
							DoError( FE_UNICODENOTALLOWED );
							Close( );
						#endif
					}
					else
					{
						SetAbsPos( 0 );
						ftype = FT_ASCII;
					}
				}
				else
				{
					SetAbsPos( 0 );
				}
				return true;
			}
			else
			{
				DoError( FE_UNABLETOOPENFILE );
				fh = NULL;
				delete[] fasciimode;
				return false;
			}
		}

        /** Closes the current open file
        */
		void Close( )
		{
			if( fh != 0 )
			{
				DoError( FE_SUCCESS );
				fclose( fh );
				fh = NULL;
			}
		}

		CFILE_BOOLEAN IsOpen( )
		{
			return (fh != NULL)?(true):(false);
		}

        /** Returns if the file has been opened in read mode
		@result
		Boolean indicating if the current file has been opened in read mode
        */
		CFILE_BOOLEAN IsRead( )
		{
			return ((fmode & FM_READ) == FM_READ)?(true):(false);
		}
        /** Returns if the file has been opened in write mode
		@result
		Boolean indicating if the current file has been opened in write mode
        */
		CFILE_BOOLEAN IsWrite( )
		{
			return ((fmode & FM_WRITE) == FM_WRITE)?(true):(false);
		}
        /** Returns filehandle
		@result
		Returns the current filehandle
        */
		FILE* FileHandle( )
		{
			return fh;
		}
        /** Returns filetype
		@result
		returns the current filetype
        */
		FILETYPE FileType( )
		{
			return ftype;
		}
		/** Returns last error
		@result
		Returns the last error occured
        */
		FILEERROR GetLastError( )
		{
			return flasterr;
		}	

        /** Moves the file position to a custom point
		@param
		offset: Indicates how many bytes to move from origin
		@param
		origin: SEEK_SET -> File start | SEEK_CUR -> Current position | SEEK_END -> Last byte in file
		@result
		Boolean indicating if the seek process has successed
		@note
		Offset can be negative, too.
        */
		CFILE_BOOLEAN Seek( CFILE_SLONG offset, signed int origin )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return false;
			}
			if( fseek( fh, offset, origin ) == 0 )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
        /** Moves the file position to an absolute position from the file start
		@param
		offset: Indicates how many bytes to move from SEEK_SET
		@result
		Boolean indicating if the seek process has successed
		@note
		Offset MUST NOT be negative, else it will not do anything
        */
		CFILE_BOOLEAN SetAbsPos( CFILE_SLONG offset )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return false;
			}
			if( offset < 0 )
			{
				DoError( FE_INTERNALERROR );
				return false;
			}
			return Seek(offset,SEEK_SET);
		}
		/** Moves the file position to the start of the file
		@result
		Boolean indicating if the rewinding process has successed
		@note
		This function checks if the file has been opened in unicode mode, and if so, it sets the pointer to 2, 
		the start of the file AFTER the unicode file header
		*/
		CFILE_BOOLEAN GoToStart( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return false;
			}
			if( ftype == FT_ASCII || ftype == FT_BINARY )
			{
				return SetAbsPos( 0 );
			}
			else if( ftype == FT_UNICODE )
			{
				return SetAbsPos( 2 );
			}
			DoError( FE_INTERNALERROR );
			return false;
		}
        /** Tells the current position in the file
		@result
		Signed long indicating the absolute offset from SEEK_SET
        */
		CFILE_SLONG Tell( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return EOF;
			}
			return ftell( fh );
		}
        /** Checks if the file position is set after the end of the file
		@result
		Boolean indicating if the file pointer is at the end of the file
        */
		CFILE_BOOLEAN EndOfFile( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return true;
			}
			return (feof( fh )==0)?(false):(true);
		}
        /** Moves the file position to a custom point
		@result
		Unsigned long indicating the size of the file in bytes
        */
		CFILE_ULONG FileSize( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			CFILE_SLONG tmp_offset = Tell( );
			if( tmp_offset < 0 ) return 0;
			Seek( 0, SEEK_END );
			CFILE_SLONG result = Tell( );
			SetAbsPos( tmp_offset );
			return result;
		}
        /** Reads from the file starting at the current position
		@param
		buf: A pointer to a buffer where the data will be stored
		@param
		element_size: The size of each block being read
		@param
		element_count: The number of blocks being read
		@result
		Unsigned int indicating the number of bytes read
        */
		CFILE_UINT Read( void* buf, CFILE_SIZE element_size, CFILE_SIZE element_count = 1 )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			return (CFILE_UINT)( fread( buf, element_size, element_count, fh ) );
		}
        /** Reads from the file starting at [offset] relative to [origin]
		@param
		buf: A pointer to a buffer where the data will be stored
		@param
		element_size: The size of each block being read
		@param
		element_count: The number of blocks being read
		@param
		offset: The offset to start reading from, relative to [origin]
		@param
		origin: Where offset is relative to, SEEK_SET -> Beginning of file | SEEK_CUR -> Current position | SEEK_END -> End of file 
		@result
		Unsigned int indicating the number of bytes read
        */
		CFILE_UINT ReadFrom( void* buf, CFILE_UINT element_size, CFILE_UINT element_count, CFILE_SLONG offset, signed int origin = SEEK_SET )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			if( !Seek( offset, origin ) ) return 0;
			return (CFILE_UINT)( Read( buf, element_size, element_count ) );
		}		
        /** Reads from the file starting at from and ending at to
		@param
		buf: A pointer to a buffer where the data will be stored
		@param
		from: The offset to start reading from, absolute, aka relative to SEEK_SET
		@param
		to: The offset to end reading at, absolute, aka relative to SEEK_SET
		@result
		Unsigned int indicating the number of bytes read
 		@note
		If (to-from) > the size of buf, it throws an exception (-> access violation), so check it before calling this function!
        */
		CFILE_UINT ReadFromTo( void* buf, CFILE_SLONG from, CFILE_SLONG to )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			if( to < from ) return 0;
			if( !SetAbsPos( from ) ) return 0;
			return (CFILE_UINT)( Read( buf, (to-from), 1 ) );
		}
        /** Reads one byte and returns it as signed char
		@result
		char with the byte read
        */
		CFILE_CHAR GetC( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return EOF;
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return EOF;
			}
			if( ftype == FT_ASCII || ftype == FT_BINARY )
			{
				return (CFILE_CHAR)( fgetc( fh ) );
			}
			else if( ftype == FT_UNICODE )
			{
				#ifndef CFILE_NOUNICODE
					CFILE_CHAR buf;
					wchar_t buffer;
					buffer = fgetwc( fh );
					#ifndef CFILE_NODEPRECATE
						size_t tmp;
						wcstombs_s( &tmp, &buf, (size_t)(sizeof(buf)), &buffer, 1 );
					#else
						wcstombs( &buf, &buffer, 1 );
					#endif
					return (CFILE_CHAR)buf;
				#else
					DoError( FE_UNICODENOTALLOWED );
					return EOF;
				#endif
			}
			return EOF;
		}

        /** Reads from the file starting at from and ending at to
		@param
		buf: A pointer to a buffer where the data will be stored
		@param
		max_count: The maximum number of bytes to read
		@result
		string with the string read
		@note
		GetS either reads up to [max_count] bytes or until the end of one line has been reached. (0x0D 0x0A)
		@note
		If the file is opened in binary mode, GetS throws an error and does nothing
        */
		CFILE_CHAR*	GetS( CRoseArray<CFILE_CHAR>* buf, CFILE_UINT max_count )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return NULL;
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return NULL;
			}
			if( ftype == FT_ASCII )
			{
				fgets( buf->arr, max_count, fh );
				return NULL;
			}
			else if( ftype == FT_UNICODE )
			{
				#ifndef CFILE_NOUNICODE
					wchar_t* buffer = new wchar_t[max_count];
					fgetws( buffer, max_count, fh );
					#ifndef CFILE_NODEPRECATE
						size_t tmp;
						wcstombs_s( &tmp, buf->arr, (size_t)(buf->absSize()), buffer, max_count );
					#else
						wcstombs( buf->arr, buffer, max_count );
					#endif
					return NULL;
				#else
					DoError( FE_UNICODENOTALLOWED );
					return NULL;
				#endif
			}
			DoError( FE_INTERNALERROR );
			return NULL;
		}		
        /** Reads one line
		@result
		string with the line read
		@note
		If the file is opened in binary mode, the call to GetS throws an error and does nothing
        */
		CFILE_CHAR*	ReadLn( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return NULL;
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return NULL;
			}
			if( EndOfFile( ) )
			{
				DoError( FE_INTERNALERROR );
				return NULL;
			}
			CRoseArray<CFILE_CHAR>* result = new CRoseArray<CFILE_CHAR>( CFILE_MAXLINELENGHT );
			result->Clean( );
			if( GetS( result, CFILE_MAXLINELENGHT ) == NULL )
			{
				CFILE_CHAR* res = new CFILE_CHAR[ (size_t)( result->size() ) ];
				for( CFILE_UINT i = 0;i<result->size(); i++ ) res[i] = 0x00;
				memcpy( res, result->arr, (size_t)(result->size()*sizeof(CFILE_CHAR)) );
				for( CFILE_UINT i = 0;i<CFILE_MAXLINELENGHT; i++ )
				{
					if( (CFILE_UCHAR)res[i] == 0x0D || (CFILE_UCHAR)res[i] == 0x0A )
					{
						if( (CFILE_ULONG)Tell( ) >= FileSize( ) - 2 )
						{
							delete result;
							delete[] res;
							return NULL;
						}
						res[i] = 0x00;
						break;
					}
				}	
				delete result;
				return res;
			}
			else
			{
				DoError( FE_INTERNALERROR );
				delete result;
				return NULL;
			}
		}
        /** Template to read different types
		@<param>
		typename
		@result
		typename: the value being read
        */
		template<typename T> T Get( )
		{
			T result;
			Read( &result, sizeof( T ), 1 );
			return result;
		}
		/*template<typename T> CVector2F Get( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return CVector2F(0,0);
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return CVector2F(0,0);
			}
			CVector2F result;
			Read( &result, sizeof( CVector2F ), 1 );
			return result;
		}	*/	
        /** Reads the whole file into the memory
		@result
		string: pointer to the memory section, NULL on error
		@note
		The current file position pointer DOES NOT get affected by this function!
        */
		/*string ReadFileToMemory( )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsRead( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			CFILE_SLONG curpos = Tell( );
			SetAbsPos( 0 );
			string mem = new char[(CFILE_SIZE)(FileSize( ))];
			if( mem == NULL ) //not enough RAM
			{
				SetAbsPos( curpos );
				return NULL;
			}
			else
			{
				for( CFILE_UINT i = 0; i < FileSize( ); i++ ) mem[i] = 0x00;
				Read( mem, (CFILE_SIZE)(FileSize( )) );
				SetAbsPos( curpos );
				return mem;
				/*
				if( Read( mem, (CFILE_SIZE)(FileSize( )) ) != FileSize( ) )
				{
					delete[] mem;
					return NULL;
				}
				else
				{
					SetAbsPos( curpos );
					return mem;
				}
				*
			}
		}*/
	
        /** Writes to the file
		@param
		ptr: A pointer to a buffer where the data will be read to write from
		@param
		element_size: The size of one block being written
		@param
		element_count: The number of blocks being written
		@result
		unsigned int indicating the number of bytes being written
        */
		CFILE_UINT Write( void* ptr, CFILE_SIZE element_size, CFILE_SIZE element_count=1 )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsWrite( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			return (CFILE_UINT)( fwrite( ptr, (size_t)( element_size ), (size_t)( element_count ), fh ) );
		}
        /** Writes to the file starting at [offset] bytes relative to [origin]
		@param
		ptr: A pointer to a buffer where the data will be read to write from
		@param
		element_size: The size of one block being written
		@param
		element_count: The number of blocks being written
		@param
		offset: The offset to start writing from, relative to [origin]
		@param
		origin: The position offset is relative to, SEEK_SET -> Start of file | SEEK_CUR -> Current position | SEEK_END -> End of file
		@result
		unsigned int indicating the number of bytes being written
        */
		CFILE_UINT WriteFrom( void* ptr, CFILE_SIZE element_size, CFILE_SIZE element_count, CFILE_SLONG offset, signed int origin )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsWrite( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			if( !Seek( offset, origin ) ) return 0;
			return (CFILE_UINT)( Write( ptr, element_size, element_count ) );
		}
        /** Writes to the file, from [from] to [to]
		@param
		ptr: A pointer to a buffer where the data will be read to write from
		@param
		from: The absolute (aka relative to SEEK_SET) offset to start writing from
		@param
		to: The absolute (aka relative to SEEK_SET) offset to stop writing at
		@result
		unsigned int indicating the number of bytes being written
		@note
		If (to-from) > the size of ptr, it throws an exception, so check it before calling this function!
        */
		CFILE_UINT WriteFromTo( void* ptr, CFILE_SLONG from, CFILE_SLONG to )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsWrite( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			if( to < from ) return 0;
			if( !SetAbsPos( from ) ) return 0;
			return (CFILE_UINT)( Write( ptr, (to-from), 1 ) );
		}
        /** Writes one byte to the file, as char
		@param
		chr: The byte to be written
		@result
		signed char: If success, it returns the char being written, else it returns EOF
        */
		CFILE_SCHAR PutC( CFILE_SCHAR chr )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return EOF;
			}
			if( !IsWrite( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return EOF;
			}
			if( ftype == FT_ASCII || ftype == FT_BINARY )
			{
				return fputc( chr, fh );
			}
			else if( ftype == FT_UNICODE )
			{
				#ifndef CFILE_NOUNICODE
					wchar_t buffer;
					#ifndef CFILE_NODEPRECATE
						size_t tmp;
						mbstowcs_s( &tmp, &buffer, 1, (CFILE_CHAR*)(&chr), 1 );
					#else
						mbstowcs( &buffer, (CFILE_CHAR*)(&chr), 1 );
					#endif
					return (CFILE_SCHAR)(fputwc( buffer, fh ));
				#else
					DoError( FE_UNICODENOTALLOWED );
					return EOF;
				#endif
			}
			return EOF;
		}

        /** Writes a string
		@param
		str: A string to the string to be written
		@result
		boolean indicating if writing the string has worked
		@note
		If the file is opened in binary mode, it throws an error and does nothing
        */
		CFILE_BOOLEAN PutS( CRoseArray<CFILE_CHAR>* str )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return false;
			}
			if( !IsWrite( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return false;
			}
			if( ftype == FT_ASCII )
			{
				return (fputs( str->arr, fh )!=EOF)?(true):(false);
			}
			else
			{
				#ifndef CFILE_NOUNICODE
					wchar_t* buffer = new wchar_t[ (size_t)(str->size()) ];
					#ifndef CFILE_NODEPRECATE
						size_t tmp;
						mbstowcs_s( &tmp, buffer, (size_t)(str->size()), str->arr, (size_t)(str->size()) );
					#else
						mbstowcs( buffer, str->arr, str->size() );
					#endif
					return (fputws( buffer, fh )!=EOF)?(true):(false);	
				#else
					DoError( FE_UNICODENOTALLOWED );
					return false;
				#endif
			}
			DoError( FE_INTERNALERROR );
			return false;
		}

        /** Template to write different types
		@<param>
		typename
		@param
		typename val: the value to be written
		@param
		cnt: times val will be written
		@result
		typename: If success, it returns the value being written, else it returns 0
        */
		template<typename T> T Put( T val, CFILE_UINT cnt=1 )
		{
			if( fh == NULL )
			{
				DoError( FE_NOFILEOPENED );
				return 0;
			}
			if( !IsWrite( ) )
			{
				DoError( FE_NORIGHTSFORACTION );
				return 0;
			}
			return (Write( &val, sizeof( val ), cnt ) != 0)?(val):(0);
		}
};
