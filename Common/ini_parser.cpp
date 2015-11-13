/*
	Ultra Light INI Parser
    Copyright (C) 2011  Grigor Margaritov
	g66666@yahoo.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	https://sourceforge.net/p/ultralightini/
*/
#include <string>
#include <string.h>
//#include <io.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include "ini_parser.h"
#ifdef _WIN32
#include <Windows.h>
#else

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#define _wopen( pString, flag, mode ) open( pString, flag, mode )
#define _chsize( file, str ) ftruncate( file, str)
#define _close( file ) close( file )
#define _O_RDWR O_RDWR
#define _S_IREAD S_IREAD
#define _S_IWRITE S_IWRITE

#endif

static const wchar_t*     g_NewLineW  = L"\r\n";
static const char*        g_NewLineA  = "\r\n";
static const unsigned int g_NewLineLn = 2;
#ifdef __UNICODE
unsigned int ReadWriteIniKeyValueStringW( const wchar_t* const lpSection, const wchar_t* const lpKey, const wchar_t* const lpDefaultValue, const wchar_t* const szPathAndFile, bool isWriteMode, wchar_t* lpReturnString, unsigned int* uReturnStringLength, bool stripValueLeadingWhitespaces, bool stripValueTrailingWhitespaces )
{
	std::basic_fstream< wchar_t >::pos_type posNewLineBeginning, posEnd;
	std::ios_base::openmode                 mode = std::ios::in;
	if ( isWriteMode )
		mode |= std::ios::out | std::ios::binary;
	std::basic_fstream< wchar_t > clFileOp( szPathAndFile, mode ); // File
	std::wstring                  result, line, section, key;
	if ( lpKey != 0 )
		key = lpKey;
	if ( lpSection != 0 )
		section = lpSection;

	int                   index;
	static const wchar_t* whitespace   = L" \t";
	bool                  foundSection = false;
	bool                  foundKey     = false;

	if ( clFileOp.is_open( ) == false )
	{
		clFileOp.clear( );
		clFileOp.open( szPathAndFile, std::ios::out ); // Create file
		clFileOp.close( );
		ReadWriteIniKeyValueStringW( lpSection, lpKey, lpDefaultValue, szPathAndFile, true, lpReturnString, uReturnStringLength, stripValueLeadingWhitespaces, stripValueTrailingWhitespaces );
		clFileOp.open( szPathAndFile, mode );

		if ( clFileOp.is_open( ) == false )
			goto lbl_end;
	}
	while ( !clFileOp.eof( ) )
	{
		posNewLineBeginning = clFileOp.tellp( );

		std::getline( clFileOp, line );
		if ( !foundSection )
		{
			//check for valid section format: " [ section_name ]"
			index = line.find_first_not_of( whitespace ); //find first non whitespace
			if ( index == std::wstring::npos )
				continue;
			if ( line[ index ] != L'[' )
				continue; //the first non whitespace char must be opening brace
			int indexSectionBegin, indexAfterSectionEnd;
			indexSectionBegin = line.find_first_not_of( whitespace, index + 1 ); //find first non whitespace char after the opening brace
			if ( indexSectionBegin == std::wstring::npos )
				continue;
			if ( lpSection != 0 )
			{
				index = line.find( lpSection, index ); //check if at that position is the searched section name
				if ( index == std::wstring::npos )
					continue;
				if ( index != indexSectionBegin )
					continue;
			}
			indexAfterSectionEnd = line.find( L']', indexSectionBegin + 1 ); //find first whitespace or closing brace char after the section name
			if ( indexAfterSectionEnd == std::wstring::npos )
				continue;
			int indexComment = line.find( L';', indexSectionBegin ); //check if there is comment in the section name
			if ( indexComment != std::wstring::npos )
				if ( indexComment <= indexAfterSectionEnd )
					continue;                                                      //if the comment is between the sectionname and the ']' (for example: [section ; ])
			index = line.find_last_not_of( whitespace, indexAfterSectionEnd - 1 ); //strip all the trailing white spaces
			if ( index != std::wstring::npos )
				indexAfterSectionEnd = index + 1;

			index = indexAfterSectionEnd - indexSectionBegin; //from here index contains now the sieze of the section
			if ( lpSection == 0 )
			{
				result.append( line.substr( indexSectionBegin, index ) );
				result.append( 1, 0 );
			}
			else
			{
				if ( index == section.size( ) ) //we need this check to make sure that between the section name and the ']' there are no more other words
					foundSection = true;
			}
		}
		else
		{
			index = line.find_first_not_of( whitespace );
			if ( index == std::wstring::npos )
				continue;
			if ( line[ index ] == L'[' ) //we have reached the next section without finding the key
			{
				//we have reached the next section without having found the key
				if ( isWriteMode )
				{
					//write here the key and the value and then write the line and all the rest of the file
					//this means:
					//starting at posNewLineBeginning
					//write key=value
					//write the rest of the file lines
					clFileOp.seekp( 0, std::ios_base::end );
					posEnd                 = clFileOp.tellp( );
					int rest_of_the_fileLn = posEnd - posNewLineBeginning - 1;
					clFileOp.seekp( posNewLineBeginning );
					wchar_t* rest_of_the_file = new wchar_t[ rest_of_the_fileLn ];
					clFileOp.read( rest_of_the_file, rest_of_the_fileLn );
					clFileOp.seekp( posNewLineBeginning );
					clFileOp.write( lpKey, wcslen( lpKey ) );
					clFileOp.write( L"=", 1 );
					clFileOp.write( lpDefaultValue, wcslen( lpDefaultValue ) );
					clFileOp.write( g_NewLineW, g_NewLineLn );
					clFileOp.write( rest_of_the_file, rest_of_the_fileLn );
					delete rest_of_the_file;
				}
				goto lbl_end;
			}
			int indexKey = index;

			if ( lpKey != 0 )
			{
				index = line.find( key, indexKey ); //check if at that position is the searched key name
				if ( index == std::wstring::npos )
					continue;
				if ( indexKey != index )
					continue; //the key must be the first non whitespace char in the line
			}
			int indexEqual;
			indexEqual = line.find( L'=', index + 1 ); //find first '=' char after the key name
			if ( indexEqual == std::wstring::npos )
				continue;
			int indexComment = line.find( L';' ); //check if there is comment prior the equal character
			if ( indexComment != std::wstring::npos )
				if ( indexComment <= indexEqual )
					continue; //if the comment is between the sectionname and the ']' (for example: [section ; ])

			int indexAfterKeyEnd = indexEqual;
			index                = line.find_last_not_of( whitespace, indexAfterKeyEnd - 1 ); //strip all the trailing white spaces
			if ( index != std::wstring::npos )
				indexAfterKeyEnd = index + 1;
			index                = indexAfterKeyEnd - indexKey; //index contains the size of the key
			if ( lpKey == 0 )
			{
				result.append( line.substr( indexKey, index ) );
				result.append( 1, 0 );
				continue;
			}
			if ( index != key.size( ) ) //we need this check to make sure that between the key name and the '=' there are no more other words
				continue;

			//now we have to get the value
			int indexValue, indexAfterValueEnd;
			indexValue = indexEqual + 1;
			if ( stripValueLeadingWhitespaces )
			{
				index = line.find_first_not_of( whitespace, indexValue ); //skip the whitespace chars between '=' and the value
				if ( index != std::wstring::npos )                        // if no value for this key has been found
					indexValue = index;
			}

			//the end of the value is eighter end of line or an unescaped comment character
			//example:
			//key = value_part1\; value_part2 ;this is the comment of this line
			//
			//search for comment on this line. An unescaped comment character marks the end of the value
			index = indexValue - 1;
			while ( ( index = line.find( L';', index + 1 ) ) != std::wstring::npos ) //search for every next comment character ';'
			{
				int  index2   = index;
				bool bEscaped = false;
				while ( line[ --index2 ] == L'\\' ) //loop back to determine if it is escaped
				{
					bEscaped = !bEscaped;
				}
				if ( !bEscaped )
					break;
				else
				{
					index--;                //decrement the index because after the erase the index of the ';' will be moved here
					line.erase( index, 1 ); //remove the escape character '\' from the combination "\;"
				}
			}
			if ( index != std::wstring::npos ) //if we didn't enter the above loop (so there is no comment character on this row)
				indexAfterValueEnd = index;
			else
				indexAfterValueEnd = line.size( );

			if ( stripValueTrailingWhitespaces && ( indexAfterValueEnd > indexValue ) )
			{
				index = line.find_last_not_of( whitespace, indexAfterValueEnd - 1 ); //strip all the trailing white spaces
				if ( index != std::wstring::npos )
					indexAfterValueEnd = index + 1;
			}

			if ( isWriteMode )
			{
				//write here the value
				//the value must be written between the indexes indexEqual and indexAfterValueEnd
				//this means:
				//starting with posNewLineBeginning
				//write line[0] to line[indexEqual]
				//write value
				//write line[indexAfterValueEnd] to line[line.size()]
				//write the rest of the file lines
				std::basic_iostream< wchar_t >::pos_type posNextLineBeginning = clFileOp.tellp( ); //this points to the next line that is not yet read
				clFileOp.seekp( 0, std::ios_base::end );
				posEnd                 = clFileOp.tellp( );
				int rest_of_the_fileLn = posEnd - posNextLineBeginning;
				clFileOp.seekp( posNextLineBeginning );
				wchar_t* rest_of_the_file = new wchar_t[ rest_of_the_fileLn ];
				clFileOp.read( rest_of_the_file, rest_of_the_fileLn );
				clFileOp.seekp( posNewLineBeginning ); //go back to the beginning of this line
				clFileOp.write( line.c_str( ), indexEqual + 1 );
				clFileOp.write( lpDefaultValue, wcslen( lpDefaultValue ) );
				clFileOp.write( &( line.c_str( )[ indexAfterValueEnd ] ), line.size( ) - indexAfterValueEnd );
				int t = line.size( ) - indexAfterValueEnd;
				if ( t < 1 )
					clFileOp.write( L"\r", 1 );
				clFileOp.write( L"\n", 1 ); //this is needed because the line object strips the \n
				clFileOp.write( rest_of_the_file, rest_of_the_fileLn );
				delete rest_of_the_file;

				//here the rest of the file must be truncated
				posNewLineBeginning = clFileOp.tellp( ); //this is the current pos
				clFileOp.seekp( 0, std::ios::end );
				posEnd = clFileOp.tellp( );
				clFileOp.seekp( 0, std::ios::beg );
				std::basic_fstream< wchar_t >::pos_type posBeginning = clFileOp.tellp( );
				int                                     fileSz       = posEnd - posNewLineBeginning;

				clFileOp.close( );

				if ( fileSz > 0 ) //if the file end is after posNewLineBeginning then we need to truncate the rest of the file
				{
					//http://msdn.microsoft.com/en-us/library/dk925tyb%28v=VS.71%29.aspx
					fileSz         = posNewLineBeginning - posBeginning; // this must be the new file size
					bool truncated = false;
#ifdef CreateFile
					HANDLE hFile = CreateFileW( szPathAndFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
					if ( hFile != INVALID_HANDLE_VALUE && hFile != 0 )
					{
						SetFilePointer( hFile, fileSz, 0, FILE_BEGIN );
						if ( SetEndOfFile( hFile ) )
							truncated = true;
						CloseHandle( hFile );
					}
#else
					int fh;
					if ( ( fh = _wopen( szPathAndFile, _O_RDWR, _S_IREAD | _S_IWRITE ) ) != -1 )
					{
						//if (ftruncate(fileno(fp), fileSz) != 0)
						if ( _chsize( fh, fileSz ) == 0 )
							truncated = true;
						_close( fh );
					}
#endif //CreateFile

					if ( !truncated )
					{
						//if the truncate failed then write white spaces from the current position till the end of the file
						rest_of_the_fileLn = posEnd - posBeginning - fileSz; //those bites must be erased
						if ( rest_of_the_fileLn < 0 )
							rest_of_the_fileLn *= -1;
						clFileOp.open( szPathAndFile, mode );
						clFileOp.seekp( posNewLineBeginning );
						clFileOp.write( g_NewLineW, g_NewLineLn );
						wchar_t* whitespaces = new wchar_t[ rest_of_the_fileLn ];
						wmemset( whitespaces, L' ', rest_of_the_fileLn );
						clFileOp.write( whitespaces, rest_of_the_fileLn );
						delete whitespaces;
						goto lbl_end;
					}
				}
				return 0;
			}
			std::wstring result = line.substr( indexValue, indexAfterValueEnd - indexValue );
			if ( lpReturnString != 0 )
			{
				if ( ( uReturnStringLength == 0 ) || ( result.length( ) < *uReturnStringLength ) )
					wcscpy( lpReturnString, result.c_str( ) );
			}
			if ( uReturnStringLength != 0 )
			{
				*uReturnStringLength = result.length( );
			}
			foundKey = true;
			break;
		}
	}

	if ( isWriteMode )
	{
		//we can end up here if we reached the end of file
		clFileOp.clear( ); //the rdstate() could be bad here if we have reached the end of file. anyway - clear it and go further
		if ( !foundSection )
		{
			//
			//write here the section, key and the value
			//this means:
			//starting with posNewLineBeginning
			//write [section]
			//write key=value
			clFileOp.write( L"\r\n[", 3 );
			clFileOp.write( lpSection, wcslen( lpSection ) );
			clFileOp.write( L"]", 1 );
		}
		//if the lpSection was the last section in the file and is empty section then
		//foundSection will be true but we still need to write the key and value
		clFileOp.write( g_NewLineW, g_NewLineLn );
		clFileOp.write( lpKey, wcslen( lpKey ) );
		clFileOp.write( L"=", 1 );
		clFileOp.write( lpDefaultValue, wcslen( lpDefaultValue ) );
		clFileOp.write( g_NewLineW, g_NewLineLn );
	}
lbl_end:
	clFileOp.close( );
	if ( isWriteMode )
	{
		return 0;
	}

	if ( lpSection == 0 || lpKey == 0 )
	{
		//result.append(L"\0"); //this doesn't work. its here only for clarity
		//result.append(L"\0"); //this doesn't work. its here only for clarity
		//so we have to add one space which later will be manually set to 0
		result.append( L" " );

		if ( lpReturnString != 0 )
		{
			if ( ( uReturnStringLength == 0 ) || ( result.size( ) < *uReturnStringLength ) )
			{
				memcpy( lpReturnString, result.c_str( ), result.size( ) * sizeof( wchar_t ) );
				lpReturnString[ result.size( ) - 1 ] = 0;
			}
		}
		if ( uReturnStringLength != 0 )
			*uReturnStringLength = result.size( );
		return result.size( );
	}
	else if ( !foundKey )
	{
		if ( lpDefaultValue != 0 )
		{
			unsigned int defaultValueLenght = wcslen( lpDefaultValue );
			if ( lpReturnString != 0 )
			{
				if ( ( uReturnStringLength == 0 ) || ( defaultValueLenght < *uReturnStringLength ) )
					wcscpy( lpReturnString, lpDefaultValue );
			}
			if ( uReturnStringLength != 0 )
				*uReturnStringLength = defaultValueLenght;
		}
		else
		{
			if ( lpReturnString != 0 )
				*lpReturnString = 0;
			if ( uReturnStringLength != 0 )
				*uReturnStringLength = 0;
		}
	}
	if ( uReturnStringLength != 0 )
		return *uReturnStringLength;
	if ( lpReturnString != 0 )
		return wcslen( lpReturnString );
	return 0;
}
#else
//this function is just copy-paste of ReadWriteIniKeyValueStringW with different types
unsigned int ReadWriteIniKeyValueStringA( const char* const lpSection, const char* const lpKey, const char* const lpDefaultValue, const char* const szPathAndFile, bool isWriteMode, char** lpReturnString, unsigned int* uReturnStringLength, bool stripValueLeadingWhitespaces, bool stripValueTrailingWhitespaces )
{
	std::basic_fstream< char >::pos_type posNewLineBeginning, posEnd;
	std::ios_base::openmode              mode = std::ios::in;
	if ( isWriteMode )
		mode |= std::ios::out | std::ios::binary;
	std::basic_fstream< char > clFileOp( szPathAndFile, mode ); // File
	std::string                result, line, section, key;
	if ( lpKey != 0 )
		key = lpKey;
	if ( lpSection != 0 )
		section = lpSection;

	uint32_t                index;
	static const char* whitespace   = " \t";
	bool               foundSection = false;
	bool               foundKey     = false;

	if ( clFileOp.is_open( ) == false )
	{
		clFileOp.clear( );
		clFileOp.open( szPathAndFile, std::ios::out ); // Create file
		clFileOp.close( );
		ReadWriteIniKeyValueStringA( lpSection, lpKey, lpDefaultValue, szPathAndFile, true, lpReturnString, uReturnStringLength, stripValueLeadingWhitespaces, stripValueTrailingWhitespaces );

		clFileOp.open( szPathAndFile, mode );

		if ( clFileOp.is_open( ) == false )
			goto lbl_end;
	}
	while ( !clFileOp.eof( ) )
	{
		posNewLineBeginning = clFileOp.tellp( );

		std::getline( clFileOp, line );
		if ( !foundSection )
		{
			//check for valid section format: " [ section_name ]"
			index = line.find_first_not_of( whitespace ); //find first non whitespace
			if ( index == std::wstring::npos )
				continue;
			if ( line[ index ] != L'[' )
				continue; //the first non whitespace char must be opening brace

			uint32_t indexSectionBegin, indexAfterSectionEnd;
			indexSectionBegin = line.find_first_not_of( whitespace, index + 1 ); //find first non whitespace char after the opening brace
			if ( indexSectionBegin == std::wstring::npos )
				continue;

			if ( lpSection != 0 )
			{
				index = line.find( lpSection, index ); //check if at that position is the searched section name
				if ( index == std::wstring::npos )
					continue;
				if ( index != indexSectionBegin )
					continue;
			}
			indexAfterSectionEnd = line.find( L']', indexSectionBegin + 1 ); //find first whitespace or closing brace char after the section name
			if ( indexAfterSectionEnd == std::wstring::npos )
				continue;

			uint32_t indexComment = line.find( L';', indexSectionBegin ); //check if there is comment in the section name
			if ( indexComment != std::wstring::npos )
				if ( indexComment <= indexAfterSectionEnd )
					continue;                                                      //if the comment is between the sectionname and the ']' (for example: [section ; ])

			index = line.find_last_not_of( whitespace, indexAfterSectionEnd - 1 ); //strip all the trailing white spaces
			if ( index != std::wstring::npos )
				indexAfterSectionEnd = index + 1;

			index = indexAfterSectionEnd - indexSectionBegin; //from here index contains now the sieze of the section
			if ( lpSection == 0 )
			{
				result.append( line.substr( indexSectionBegin, index ) );
				result.append( 1, 0 );
			}
			else
			{
				if ( index == section.size( ) ) //we need this check to make sure that between the section name and the ']' there are no more other words
					foundSection = true;
			}
		}
		else
		{
			index = line.find_first_not_of( whitespace );
			if ( index == std::wstring::npos )
				continue;

			if ( line[ index ] == L'[' ) //we have reached the next section without finding the key
			{
				//we have reached the next section without having found the key
				if ( isWriteMode )
				{
					//write here the key and the value and then write the line and all the rest of the file
					//this means:
					//starting at posNewLineBeginning
					//write key=value
					//write the rest of the file lines
					clFileOp.seekp( 0, std::ios_base::end );
					posEnd                 = clFileOp.tellp( );
					int rest_of_the_fileLn = posEnd - posNewLineBeginning - 1;
					clFileOp.seekp( posNewLineBeginning );
					char* rest_of_the_file = new char[ rest_of_the_fileLn ];
					clFileOp.read( rest_of_the_file, rest_of_the_fileLn );
					clFileOp.seekp( posNewLineBeginning );
					clFileOp.write( lpKey, strlen( lpKey ) );
					clFileOp.write( "=", 1 );
					clFileOp.write( lpDefaultValue, strlen( lpDefaultValue ) );
					clFileOp.write( g_NewLineA, g_NewLineLn );
					clFileOp.write( rest_of_the_file, rest_of_the_fileLn );
					delete rest_of_the_file;
				}
				goto lbl_end;
			}

			uint32_t indexKey = index;

			if ( lpKey != 0 )
			{
				index = line.find( key, indexKey ); //check if at that position is the searched key name
				if ( index == std::wstring::npos )
					continue;
				if ( indexKey != index )
					continue; //the key must be the first non whitespace char in the line
			}

			uint32_t indexEqual;
			indexEqual = line.find( L'=', index + 1 ); //find first '=' char after the key name
			if ( indexEqual == std::wstring::npos )
				continue;

			uint32_t indexComment = line.find( L';' ); //check if there is comment prior the equal character
			if ( indexComment != std::wstring::npos )
				if ( indexComment <= indexEqual )
					continue; //if the comment is between the sectionname and the ']' (for example: [section ; ])

			uint32_t indexAfterKeyEnd = indexEqual;
			index                = line.find_last_not_of( whitespace, indexAfterKeyEnd - 1 ); //strip all the trailing white spaces
			if ( index != std::wstring::npos )
				indexAfterKeyEnd = index + 1;

			index                = indexAfterKeyEnd - indexKey; //index contains the size of the key
			if ( lpKey == 0 )
			{
				result.append( line.substr( indexKey, index ) );
				result.append( 1, 0 );
				continue;
			}

			if ( index != key.size( ) ) //we need this check to make sure that between the key name and the '=' there are no more other words
				continue;

			//now we have to get the value
			uint32_t indexValue, indexAfterValueEnd;
			indexValue = indexEqual + 1;
			if ( stripValueLeadingWhitespaces )
			{
				index = line.find_first_not_of( whitespace, indexValue ); //skip the whitespace chars between '=' and the value
				if ( index != std::wstring::npos )                        // if no value for this key has been found
					indexValue = index;
			}

			//the end of the value is eighter end of line or an unescaped comment character
			//example:
			//key = value_part1\; value_part2 ;this is the comment of this line
			//
			//search for comment on this line. An unescaped comment character marks the end of the value
			index = indexValue - 1;
			while ( ( index = line.find( L';', index + 1 ) ) != std::wstring::npos ) //search for every next comment character ';'
			{
				int  index2   = index;
				bool bEscaped = false;
				while ( line[ --index2 ] == L'\\' ) //loop back to determine if it is escaped
				{
					bEscaped = !bEscaped;
				}
				if ( !bEscaped )
					break;
				else
				{
					index--;                //decrement the index because after the erase the index of the ';' will be moved here
					line.erase( index, 1 ); //remove the escape character '\' from the combination "\;"
				}
			}
			if ( index != std::wstring::npos ) //if we didn't enter the above loop (so there is no comment character on this row)
				indexAfterValueEnd = index;
			else
				indexAfterValueEnd = line.size( );

			if ( stripValueTrailingWhitespaces && ( indexAfterValueEnd > indexValue ) )
			{
				index = line.find_last_not_of( whitespace, indexAfterValueEnd - 1 ); //strip all the trailing white spaces
				if ( index != std::wstring::npos )
					indexAfterValueEnd = index + 1;
			}

			if ( isWriteMode )
			{
				//write here the value
				//the value must be written between the indexes indexEqual and indexAfterValueEnd
				//this means:
				//starting with posNewLineBeginning
				//write line[0] to line[indexEqual]
				//write value
				//write line[indexAfterValueEnd] to line[line.size()]
				//write the rest of the file lines
				std::basic_iostream< char >::pos_type posNextLineBeginning = clFileOp.tellp( ); //this points to the next line that is not yet read
				clFileOp.seekp( 0, std::ios_base::end );
				posEnd                 = clFileOp.tellp( );
				int rest_of_the_fileLn = posEnd - posNextLineBeginning;
				clFileOp.seekp( posNextLineBeginning );
				char* rest_of_the_file = new char[ rest_of_the_fileLn ];
				clFileOp.read( rest_of_the_file, rest_of_the_fileLn );
				clFileOp.seekp( posNewLineBeginning ); //go back to the beginning of this line
				clFileOp.write( line.c_str( ), indexEqual + 1 );
				clFileOp.write( lpDefaultValue, strlen( lpDefaultValue ) );
				clFileOp.write( &( line.c_str( )[ indexAfterValueEnd ] ), line.size( ) - indexAfterValueEnd );
				int t = line.size( ) - indexAfterValueEnd;
				if ( t < 1 )
					clFileOp.write( "\r", 1 );
				clFileOp.write( "\n", 1 ); //this is needed because the line object strips the \n
				clFileOp.write( rest_of_the_file, rest_of_the_fileLn );
				delete rest_of_the_file;

				//here the rest of the file must be truncated
				posNewLineBeginning = clFileOp.tellp( ); //this is the current pos
				clFileOp.seekp( 0, std::ios::end );
				posEnd = clFileOp.tellp( );
				clFileOp.seekp( 0, std::ios::beg );
				std::basic_fstream< char >::pos_type posBeginning = clFileOp.tellp( );
				int                                  fileSz       = posEnd - posNewLineBeginning;

				clFileOp.close( );

				if ( fileSz > 0 ) //if the file end is after posNewLineBeginning then we need to truncate the rest of the file
				{
					//http://msdn.microsoft.com/en-us/library/dk925tyb%28v=VS.71%29.aspx
					fileSz         = posNewLineBeginning - posBeginning; // this must be the new file size
					bool truncated = false;
#ifdef CreateFile
					HANDLE hFile = CreateFileA( szPathAndFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
					if ( hFile != INVALID_HANDLE_VALUE && hFile != 0 )
					{
						SetFilePointer( hFile, fileSz, 0, FILE_BEGIN );
						if ( SetEndOfFile( hFile ) )
							truncated = true;
						CloseHandle( hFile );
					}
#else
					int fh;
					if ( ( fh = _wopen( szPathAndFile, _O_RDWR, _S_IREAD | _S_IWRITE ) ) != -1 )
					{
						//if (ftruncate(fileno(fp), fileSz) != 0)
						if ( _chsize( fh, fileSz ) == 0 )
							truncated = true;
						_close( fh );
					}
#endif //CreateFile

					if ( !truncated )
					{
						//if the truncate failed then write white spaces from the current position till the end of the file
						rest_of_the_fileLn = posEnd - posBeginning - fileSz; //those bites must be erased
						if ( rest_of_the_fileLn < 0 )
							rest_of_the_fileLn *= -1;
						clFileOp.open( szPathAndFile, mode );
						clFileOp.seekp( posNewLineBeginning );
						clFileOp.write( g_NewLineA, g_NewLineLn );
						char* whitespaces = new char[ rest_of_the_fileLn ];
						memset( whitespaces, ' ', rest_of_the_fileLn );
						clFileOp.write( whitespaces, rest_of_the_fileLn );
						delete whitespaces;
						goto lbl_end;
					}
				}
				return 0;
			}
			std::string result = line.substr( indexValue, indexAfterValueEnd - indexValue );
			if ( lpReturnString != 0 )
			{
				if ( ( uReturnStringLength == 0 ) || ( result.length( ) < *uReturnStringLength ) )
				{
					*lpReturnString = new char[ result.size( ) ];

					if ( *lpReturnString != nullptr )
						strcpy( *lpReturnString, result.c_str( ) );
				}
			}
			if ( uReturnStringLength != 0 )
			{
				*uReturnStringLength = result.length( );
			}
			foundKey = true;
			break;
		}
	}

	if ( isWriteMode )
	{
		//we can end up here if we reached the end of file
		clFileOp.clear( ); //the rdstate() could be bad here if we have reached the end of file. anyway - clear it and go further
		if ( !foundSection )
		{
			//
			//write here the section, key and the value
			//this means:
			//starting with posNewLineBeginning
			//write [section]
			//write key=value
			clFileOp.write( "\r\n[", 3 );
			clFileOp.write( lpSection, strlen( lpSection ) );
			clFileOp.write( "]", 1 );
		}
		//if the lpSection was the last section in the file and is empty section then
		//foundSection will be true but we still need to write the key and value
		clFileOp.write( g_NewLineA, g_NewLineLn );
		clFileOp.write( lpKey, strlen( lpKey ) );
		clFileOp.write( "=", 1 );
		clFileOp.write( lpDefaultValue, strlen( lpDefaultValue ) );
		clFileOp.write( g_NewLineA, g_NewLineLn );
	}
lbl_end:
	clFileOp.close( );
	if ( isWriteMode )
	{
		return 0;
	}

	if ( lpSection == 0 || lpKey == 0 )
	{
		//result.append("\0"); //this doesn't work. its here only for clarity
		//result.append("\0"); //this doesn't work. its here only for clarity
		//so we have to add one space which later will be manually set to 0
		result.append( " " );

		if ( lpReturnString != 0 )
		{
			if ( ( uReturnStringLength == 0 ) || ( result.size( ) < *uReturnStringLength ) )
			{
				memcpy( lpReturnString, result.c_str( ), result.size( ) * sizeof( char ) );
				lpReturnString[ result.size( ) - 1 ] = 0;
			}
		}
		if ( uReturnStringLength != 0 )
			*uReturnStringLength = result.size( );
		return result.size( );
	}
	else if ( !foundKey )
	{
		if ( lpDefaultValue != 0 )
		{
			unsigned int defaultValueLenght = strlen( lpDefaultValue );
			if ( lpReturnString != 0 )
			{
				if ( ( uReturnStringLength == 0 ) || ( defaultValueLenght < *uReturnStringLength ) )
				{
					*lpReturnString = new char[ result.size( ) ];
					if ( *lpReturnString != nullptr )
						strcpy( *lpReturnString, lpDefaultValue );
				}
			}
			if ( uReturnStringLength != 0 )
				*uReturnStringLength = defaultValueLenght;
		}
		else
		{
			if ( lpReturnString != 0 )
				*lpReturnString = 0;
			if ( uReturnStringLength != 0 )
				*uReturnStringLength = 0;
		}
	}
	if ( uReturnStringLength != 0 )
		return *uReturnStringLength;
	if ( lpReturnString != 0 )
		return strlen( *lpReturnString );
	return 0;
}

int ReadWriteIniKeyValueInt( const char* const lpSection, const char* const lpKey, const int lpDefaultValue, const char* const szPathAndFile, bool isWriteMode, int* lpReturnInt )
{
	std::string                          pWVal = std::to_string( lpDefaultValue );
	std::basic_fstream< char >::pos_type posNewLineBeginning, posEnd;
	std::ios_base::openmode              mode = std::ios::in;
	if ( isWriteMode )
		mode |= std::ios::out | std::ios::binary;
	std::basic_fstream< char > clFileOp( szPathAndFile, mode ); // File
	std::string                result, line, section, key;
	if ( lpKey != 0 )
		key = lpKey;
	if ( lpSection != 0 )
		section = lpSection;

	int                index;
	static const char* whitespace   = " \t";
	bool               foundSection = false;
	bool               foundKey     = false;

	if ( clFileOp.is_open( ) == false )
	{
		clFileOp.clear( );
		clFileOp.open( szPathAndFile, std::ios::out ); // Create file
		clFileOp.close( );

		ReadWriteIniKeyValueInt( lpSection, lpKey, lpDefaultValue, szPathAndFile, true, lpReturnInt );

		clFileOp.open( szPathAndFile, mode );

		if ( clFileOp.is_open( ) == false )
			goto lbl_end;
	}
	while ( !clFileOp.eof( ) )
	{
		posNewLineBeginning = clFileOp.tellp( );

		std::getline( clFileOp, line );
		if ( !foundSection )
		{
			//check for valid section format: " [ section_name ]"
			index = line.find_first_not_of( whitespace ); //find first non whitespace
			if ( index == std::wstring::npos )
				continue;
			if ( line[ index ] != L'[' )
				continue; //the first non whitespace char must be opening brace
			int indexSectionBegin, indexAfterSectionEnd;
			indexSectionBegin = line.find_first_not_of( whitespace, index + 1 ); //find first non whitespace char after the opening brace
			if ( indexSectionBegin == std::wstring::npos )
				continue;
			if ( lpSection != 0 )
			{
				index = line.find( lpSection, index ); //check if at that position is the searched section name
				if ( index == std::wstring::npos )
					continue;
				if ( index != indexSectionBegin )
					continue;
			}
			indexAfterSectionEnd = line.find( L']', indexSectionBegin + 1 ); //find first whitespace or closing brace char after the section name
			if ( indexAfterSectionEnd == std::wstring::npos )
				continue;
			int indexComment = line.find( L';', indexSectionBegin ); //check if there is comment in the section name
			if ( indexComment != std::wstring::npos )
				if ( indexComment <= indexAfterSectionEnd )
					continue;                                                      //if the comment is between the sectionname and the ']' (for example: [section ; ])
			index = line.find_last_not_of( whitespace, indexAfterSectionEnd - 1 ); //strip all the trailing white spaces
			if ( index != std::wstring::npos )
				indexAfterSectionEnd = index + 1;

			index = indexAfterSectionEnd - indexSectionBegin; //from here index contains now the sieze of the section
			if ( lpSection == 0 )
			{
				result.append( line.substr( indexSectionBegin, index ) );
				result.append( 1, 0 );
			}
			else
			{
				if ( index == section.size( ) ) //we need this check to make sure that between the section name and the ']' there are no more other words
					foundSection = true;
			}
		}
		else
		{
			index = line.find_first_not_of( whitespace );
			if ( index == std::wstring::npos )
				continue;
			if ( line[ index ] == L'[' ) //we have reached the next section without finding the key
			{
				//we have reached the next section without having found the key
				if ( isWriteMode )
				{
					//write here the key and the value and then write the line and all the rest of the file
					//this means:
					//starting at posNewLineBeginning
					//write key=value
					//write the rest of the file lines
					clFileOp.seekp( 0, std::ios_base::end );
					posEnd                 = clFileOp.tellp( );
					int rest_of_the_fileLn = posEnd - posNewLineBeginning - 1;
					clFileOp.seekp( posNewLineBeginning );
					char* rest_of_the_file = new char[ rest_of_the_fileLn ];
					clFileOp.read( rest_of_the_file, rest_of_the_fileLn );
					clFileOp.seekp( posNewLineBeginning );
					clFileOp.write( lpKey, strlen( lpKey ) );
					clFileOp.write( "=", 1 );

					std::string pWVal = std::to_string( lpDefaultValue );
					clFileOp.write( pWVal.c_str( ), pWVal.size( ) );
					clFileOp.write( g_NewLineA, g_NewLineLn );
					clFileOp.write( rest_of_the_file, rest_of_the_fileLn );
					delete rest_of_the_file;
				}
				goto lbl_end;
			}
			int indexKey = index;

			if ( lpKey != 0 )
			{
				index = line.find( key, indexKey ); //check if at that position is the searched key name
				if ( index == std::wstring::npos )
					continue;
				if ( indexKey != index )
					continue; //the key must be the first non whitespace char in the line
			}
			int indexEqual;
			indexEqual = line.find( L'=', index + 1 ); //find first '=' char after the key name
			if ( indexEqual == std::wstring::npos )
				continue;
			int indexComment = line.find( L';' ); //check if there is comment prior the equal character
			if ( indexComment != std::wstring::npos )
				if ( indexComment <= indexEqual )
					continue; //if the comment is between the sectionname and the ']' (for example: [section ; ])

			int indexAfterKeyEnd = indexEqual;
			index                = line.find_last_not_of( whitespace, indexAfterKeyEnd - 1 ); //strip all the trailing white spaces
			if ( index != std::wstring::npos )
				indexAfterKeyEnd = index + 1;
			index                = indexAfterKeyEnd - indexKey; //index contains the size of the key
			if ( lpKey == 0 )
			{
				result.append( line.substr( indexKey, index ) );
				result.append( 1, 0 );
				continue;
			}
			if ( index != key.size( ) ) //we need this check to make sure that between the key name and the '=' there are no more other words
				continue;

			//now we have to get the value
			int indexValue, indexAfterValueEnd;
			indexValue = indexEqual + 1;
			//if (stripValueLeadingWhitespaces)
			{
				index = line.find_first_not_of( whitespace, indexValue ); //skip the whitespace chars between '=' and the value
				if ( index != std::wstring::npos )                        // if no value for this key has been found
					indexValue = index;
			}

			//the end of the value is eighter end of line or an unescaped comment character
			//example:
			//key = value_part1\; value_part2 ;this is the comment of this line
			//
			//search for comment on this line. An unescaped comment character marks the end of the value
			index = indexValue - 1;
			while ( ( index = line.find( L';', index + 1 ) ) != std::wstring::npos ) //search for every next comment character ';'
			{
				int  index2   = index;
				bool bEscaped = false;
				while ( line[ --index2 ] == L'\\' ) //loop back to determine if it is escaped
				{
					bEscaped = !bEscaped;
				}
				if ( !bEscaped )
					break;
				else
				{
					index--;                //decrement the index because after the erase the index of the ';' will be moved here
					line.erase( index, 1 ); //remove the escape character '\' from the combination "\;"
				}
			}
			if ( index != std::wstring::npos ) //if we didn't enter the above loop (so there is no comment character on this row)
				indexAfterValueEnd = index;
			else
				indexAfterValueEnd = line.size( );

			if ( ( indexAfterValueEnd > indexValue ) )
			{
				index = line.find_last_not_of( whitespace, indexAfterValueEnd - 1 ); //strip all the trailing white spaces
				if ( index != std::wstring::npos )
					indexAfterValueEnd = index + 1;
			}

			if ( isWriteMode )
			{
				//write here the value
				//the value must be written between the indexes indexEqual and indexAfterValueEnd
				//this means:
				//starting with posNewLineBeginning
				//write line[0] to line[indexEqual]
				//write value
				//write line[indexAfterValueEnd] to line[line.size()]
				//write the rest of the file lines
				std::basic_iostream< char >::pos_type posNextLineBeginning = clFileOp.tellp( ); //this points to the next line that is not yet read
				clFileOp.seekp( 0, std::ios_base::end );
				posEnd                 = clFileOp.tellp( );
				int rest_of_the_fileLn = posEnd - posNextLineBeginning;
				clFileOp.seekp( posNextLineBeginning );
				char* rest_of_the_file = new char[ rest_of_the_fileLn ];
				clFileOp.read( rest_of_the_file, rest_of_the_fileLn );
				clFileOp.seekp( posNewLineBeginning ); //go back to the beginning of this line
				clFileOp.write( line.c_str( ), indexEqual + 1 );
				clFileOp.write( pWVal.c_str( ), pWVal.size( ) );
				clFileOp.write( &( line.c_str( )[ indexAfterValueEnd ] ), line.size( ) - indexAfterValueEnd );
				int t = line.size( ) - indexAfterValueEnd;
				if ( t < 1 )
					clFileOp.write( "\r", 1 );
				clFileOp.write( "\n", 1 ); //this is needed because the line object strips the \n
				clFileOp.write( rest_of_the_file, rest_of_the_fileLn );
				delete rest_of_the_file;

				//here the rest of the file must be truncated
				posNewLineBeginning = clFileOp.tellp( ); //this is the current pos
				clFileOp.seekp( 0, std::ios::end );
				posEnd = clFileOp.tellp( );
				clFileOp.seekp( 0, std::ios::beg );
				std::basic_fstream< char >::pos_type posBeginning = clFileOp.tellp( );
				int                                  fileSz       = posEnd - posNewLineBeginning;

				clFileOp.close( );

				if ( fileSz > 0 ) //if the file end is after posNewLineBeginning then we need to truncate the rest of the file
				{
					//http://msdn.microsoft.com/en-us/library/dk925tyb%28v=VS.71%29.aspx
					fileSz         = posNewLineBeginning - posBeginning; // this must be the new file size
					bool truncated = false;
#ifdef CreateFile
					HANDLE hFile = CreateFileA( szPathAndFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
					if ( hFile != INVALID_HANDLE_VALUE && hFile != 0 )
					{
						SetFilePointer( hFile, fileSz, 0, FILE_BEGIN );
						if ( SetEndOfFile( hFile ) )
							truncated = true;
						CloseHandle( hFile );
					}
#else
					int fh;
					if ( ( fh = _wopen( szPathAndFile, _O_RDWR, _S_IREAD | _S_IWRITE ) ) != -1 )
					{
						//if (ftruncate(fileno(fp), fileSz) != 0)
						if ( _chsize( fh, fileSz ) == 0 )
							truncated = true;
						_close( fh );
					}
#endif //CreateFile

					if ( !truncated )
					{
						//if the truncate failed then write white spaces from the current position till the end of the file
						rest_of_the_fileLn = posEnd - posBeginning - fileSz; //those bites must be erased
						if ( rest_of_the_fileLn < 0 )
							rest_of_the_fileLn *= -1;
						clFileOp.open( szPathAndFile, mode );
						clFileOp.seekp( posNewLineBeginning );
						clFileOp.write( g_NewLineA, g_NewLineLn );
						char* whitespaces = new char[ rest_of_the_fileLn ];
						memset( whitespaces, ' ', rest_of_the_fileLn );
						clFileOp.write( whitespaces, rest_of_the_fileLn );
						delete whitespaces;
						goto lbl_end;
					}
				}
				return 0;
			}
			result   = line.substr( indexValue, indexAfterValueEnd - indexValue );
			foundKey = true;
			break;
		}
	} //END WHILE

	if ( isWriteMode )
	{
		//we can end up here if we reached the end of file
		clFileOp.clear( ); //the rdstate() could be bad here if we have reached the end of file. anyway - clear it and go further
		if ( !foundSection )
		{
			//
			//write here the section, key and the value
			//this means:
			//starting with posNewLineBeginning
			//write [section]
			//write key=value
			clFileOp.write( "\r\n[", 3 );
			clFileOp.write( lpSection, strlen( lpSection ) );
			clFileOp.write( "]", 1 );
		}
		//if the lpSection was the last section in the file and is empty section then
		//foundSection will be true but we still need to write the key and value
		clFileOp.write( g_NewLineA, g_NewLineLn );
		clFileOp.write( lpKey, strlen( lpKey ) );
		clFileOp.write( "=", 1 );
		clFileOp.write( pWVal.c_str( ), pWVal.size( ) );
		clFileOp.write( g_NewLineA, g_NewLineLn );
	}
lbl_end:
	clFileOp.close( );
	if ( isWriteMode )
	{
		return 0;
	}

	if ( lpSection == 0 || lpKey == 0 )
	{
		return lpDefaultValue;
	}

	if ( lpReturnInt != nullptr )
	{
		*lpReturnInt = std::stoi( result );
	}

	return std::stoi( result );
}
#endif
/*
bool testIniParserW()
{
	bool testfailed = false;
	wchar_t* settingsFile = L"C:\\Grigor\\UltraLightTextEditor\\testW.ini";
	wchar_t returnString[100];
	memset(returnString, 1, 100*sizeof(wchar_t));
	
	unsigned int result;
	unsigned int uReturnStringLength;
	
	//WRITE INTO THE FILE
	//the test of writing is checked later by the reading. 
	//if you can read from the file then obviously the write has succeded
	result = ReadWriteIniKeyValueStringW(L"s10", L"k10", L"val10", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin section and key
	result = ReadWriteIniKeyValueStringW(L"s10", L"k 11", L"val11", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin key
	result = ReadWriteIniKeyValueStringW(L"s10", L"k10", L"val12", settingsFile, true, returnString, &uReturnStringLength, true, true);		//overwite value in existing section and key

	result = ReadWriteIniKeyValueStringW(L"s20", L"k20", L"val20;comment", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin section and key
	result = ReadWriteIniKeyValueStringW(L"s20", L"k21", L"val21", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin key
	result = ReadWriteIniKeyValueStringW(L"s20", L"k20", L"val22 val23 val24", settingsFile, true, returnString, &uReturnStringLength, true, true);		//overwite value with whitespaces in existing section and key. the existing comment must not be overwritten!
	result = ReadWriteIniKeyValueStringW(L"s20", L"k21", L" val21 val212  ;this is comment and the value is with trailing and leading white spaces", settingsFile, true, returnString, &uReturnStringLength, true, true);
	
	//READ FROM THE FILE
	//test get non existing value
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s10", L"non existing key", L"default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (wcscmp(returnString, L"default") != 0 || uReturnStringLength != result || uReturnStringLength != 7)
		testfailed = true;

	//test get non existing value without default string
	uReturnStringLength = 100;
	returnString[0] = 0;
	result = ReadWriteIniKeyValueStringW(L"s10", L"non existing key", 0, settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (returnString[0] != 0 || uReturnStringLength != result || uReturnStringLength != 0)
		testfailed = true;
		
	//test get value
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s10", L"k10", L"default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (wcscmp(returnString, L"val12") != 0 || uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get value with usufficient buffer (small uReturnStringLength)
	uReturnStringLength = 1;
	result = ReadWriteIniKeyValueStringW(L"s10", L"k10", L"default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (wcscmp(returnString, L"val12") != 0 || uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get only the lenght of the value (no returnString)
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s10", L"k10", L"default", settingsFile, false, 0, &uReturnStringLength, true, true);
	if (uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get value that was over written; get value of key with white space in its name
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s10", L"k 11", L"default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (wcscmp(returnString, L"val11") != 0 || uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get value with white spaces in it
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s20", L"k20", L"default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (wcscmp(returnString, L"val22 val23 val24") != 0 || uReturnStringLength != result || uReturnStringLength != 17)
		testfailed = true;
	
	//test get value without uReturnStringLength
	//uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s20", L"k21", L"default", settingsFile, false, returnString, 0, true, true);
	if (wcscmp(returnString, L"val21 val212") != 0 || result != 12)
		testfailed = true;
	
	//test get value with leading and trailing characters
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s20", L"k21", L"default", settingsFile, false, returnString, &uReturnStringLength, false, false);
	if (wcscmp(returnString, L" val21 val212  ") != 0 || uReturnStringLength != result || uReturnStringLength != 15)
		testfailed = true;
	
	//test get all the keys in one section
	memset(returnString, 1, 100*sizeof(wchar_t));
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(L"s20", 0, L"default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	wchar_t* keys_to_return = L"k20\0k21\0\0";
	if (memcmp(returnString, keys_to_return, result*sizeof(wchar_t)) != 0 || uReturnStringLength != result || uReturnStringLength != 9)
		testfailed = true;
		
	//test get all the sections in the file
	memset(returnString, 1, 100*sizeof(wchar_t));
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringW(0, 0, L"default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	wchar_t* sections_to_return = L"s10\0s20\0\0";
	if (memcmp(returnString, sections_to_return, result*sizeof(wchar_t)) != 0 || uReturnStringLength != result || uReturnStringLength != 9)
		testfailed = true;
	return testfailed;
}

bool testIniParserA()
{
	bool testfailed = false;
	char* settingsFile = "C:\\Grigor\\UltraLightTextEditor\\testA.ini";
	char returnString[100];
	memset(returnString, 1, 100*sizeof(char));
	
	unsigned int result;
	unsigned int uReturnStringLength;
	
	//WRITE INTO THE FILE
	//the test of writing is checked later by the reading. 
	//if you can read from the file then obviously the write has succeded
	result = ReadWriteIniKeyValueStringA("s10", "k10", "val10", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin section and key
	result = ReadWriteIniKeyValueStringA("s10", "k 11", "val11", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin key
	result = ReadWriteIniKeyValueStringA("s10", "k10", "val12", settingsFile, true, returnString, &uReturnStringLength, true, true);		//overwite value in existing section and key

	result = ReadWriteIniKeyValueStringA("s20", "k20", "val20;comment", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin section and key
	result = ReadWriteIniKeyValueStringA("s20", "k21", "val21", settingsFile, true, returnString, &uReturnStringLength, true, true);		//write nonexistin key
	result = ReadWriteIniKeyValueStringA("s20", "k20", "val22 val23 val24", settingsFile, true, returnString, &uReturnStringLength, true, true);		//overwite value with whitespaces in existing section and key. the existing comment must not be overwritten!
	result = ReadWriteIniKeyValueStringA("s20", "k21", " val21 val212  ;this is comment and the value is with trailing and leading white spaces", settingsFile, true, returnString, &uReturnStringLength, true, true);
	
	//READ FROM THE FILE
	//test get non existing value
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s10", "non existing key", "default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (strcmp(returnString, "default") != 0 || uReturnStringLength != result || uReturnStringLength != 7)
		testfailed = true;

	//test get non existing value without default string
	uReturnStringLength = 100;
	returnString[0] = 0;
	result = ReadWriteIniKeyValueStringA("s10", "non existing key", 0, settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (returnString[0] != 0 || uReturnStringLength != result || uReturnStringLength != 0)
		testfailed = true;
		
	//test get value
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s10", "k10", "default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (strcmp(returnString, "val12") != 0 || uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get value with usufficient buffer (small uReturnStringLength)
	uReturnStringLength = 1;
	result = ReadWriteIniKeyValueStringA("s10", "k10", "default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (strcmp(returnString, "val12") != 0 || uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get only the lenght of the value (no returnString)
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s10", "k10", "default", settingsFile, false, 0, &uReturnStringLength, true, true);
	if (uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get value that was over written; get value of key with white space in its name
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s10", "k 11", "default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (strcmp(returnString, "val11") != 0 || uReturnStringLength != result || uReturnStringLength != 5)
		testfailed = true;
	
	//test get value with white spaces in it
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s20", "k20", "default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	if (strcmp(returnString, "val22 val23 val24") != 0 || uReturnStringLength != result || uReturnStringLength != 17)
		testfailed = true;
	
	//test get value without uReturnStringLength
	//uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s20", "k21", "default", settingsFile, false, returnString, 0, true, true);
	if (strcmp(returnString, "val21 val212") != 0 || result != 12)
		testfailed = true;
	
	//test get value with leading and trailing characters
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s20", "k21", "default", settingsFile, false, returnString, &uReturnStringLength, false, false);
	if (strcmp(returnString, " val21 val212  ") != 0 || uReturnStringLength != result || uReturnStringLength != 15)
		testfailed = true;
	
	//test get all the keys in one section
	memset(returnString, 1, 100*sizeof(char));
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA("s20", 0, "default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	char* keys_to_return = "k20\0k21\0\0";
	if (memcmp(returnString, keys_to_return, result*sizeof(char)) != 0 || uReturnStringLength != result || uReturnStringLength != 9)
		testfailed = true;
		
	//test get all the sections in the file
	memset(returnString, 1, 100*sizeof(char));
	uReturnStringLength = 100;
	result = ReadWriteIniKeyValueStringA(0, 0, "default", settingsFile, false, returnString, &uReturnStringLength, true, true);
	char* sections_to_return = "s10\0s20\0\0";
	if (memcmp(returnString, sections_to_return, result*sizeof(char)) != 0 || uReturnStringLength != result || uReturnStringLength != 9)
		testfailed = true;
	return testfailed;
}
*/
