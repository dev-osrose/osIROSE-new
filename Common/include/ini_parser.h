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
#ifndef _ini_parser_h_
#define _ini_parser_h_

//function ReadWriteIniKeyValueStringW reads\writes string from\to specified section in INI file
//if isWriteMode == true then the function writes the string pointed by lpDefaultValue into the ini file.
//lpReturnString, uReturnStringLength, stripValueLeadingWhitespaces, stripValueTrailingWhitespaces are ignored
//lpDefaultValue contains the value to be set
//   tip: you can write comment after the value. You just need to append the comment after the value string.
//   like this: lpDefaultValue = "this is the value to write; this is comment after the value";
//return 0.
//
//
//
//if isWriteMode == false then the function reads the value of key lpKey in section lpSection.
//if the key does not exist
//                        and lpDefaultValue is not null then lpDefaultValue is copied in lpReturnString
//                        and lpDefaultValue is null then is returned 0
//lpSection => section to read from.
//             If NULL the result is a string with all the sections of the ini file with with '\0' between them and double terminatin null character at the end "\0\0"
//             result example: "section1\0section2\0secion3\0\0"
//lpKey => key to read from.
//             If NULL the result is a string with all the keys in the given section of the ini file with with '\0' between them and double terminatin null character at the end "\0\0"
//             result example: "key1\0key2\0key3\0\0"
//lpReturnString => pointer to allocated buffer that will receive the read value. If NULL, the uReturnStringLength will only receive the number of characters lpReturnString must allocate to read the value
//uReturnStringLength => [in] contains the length in characters of the buffer pointed by lpReturnString.
//             if NULL then is assumed that the lpReturnString is sufficient to hold the data
//             [out] contains the length in characters of the string read from the ini file not including the terminating null character
//stripValueLeadingWhitespaces => flag indicating if the leading whitespaces of the value will be included in the returned string.
//                                If true then the whitspaces are excluded from the result
//                                example: key1= value1    ;there is one whitespace between the '=' and value1
//stripValueTrailingWhitespaces => flag indicating if the trailing whitespaces of the value will be included in the returned string.
//                                If true then the whitspaces are excluded from the result
//                                example: key1= value1  ;there is two whitespaces between the end of value1 and the commentars starting character ';'
//if stripValueLeadingWhitespaces and stripValueTrailingWhitespaces are both false then the value can be whitespace (' ' or '\t')
//
//return result:
//       if (lpReturnString != NULL) returns the length in characters of the string copied in lpReturnString.
//       else returns the length in characters of the string. This result can be used to allocate memory for lpReturnString.
//       The return result does not include the terminating null character!
unsigned int ReadWriteIniKeyValueStringW( const wchar_t* const lpSection, const wchar_t* const lpKey, const wchar_t* const lpDefaultValue, const wchar_t* const szPathAndFile, bool isWriteMode = false, wchar_t* lpReturnString = 0, unsigned int* uReturnStringLength = 0, bool stripValueLeadingWhitespaces = true, bool stripValueTrailingWhitespaces = true );

//the same as ReadWriteIniKeyValueStringW but writes chars
unsigned int ReadWriteIniKeyValueStringA( const char* const lpSection, const char* const lpKey, const char* const lpDefaultValue, const char* const szPathAndFile, bool isWriteMode = false, char** lpReturnString = 0, unsigned int* uReturnStringLength = 0, bool stripValueLeadingWhitespaces = true, bool stripValueTrailingWhitespaces = true );
int ReadWriteIniKeyValueInt( const char* const lpSection, const char* const lpKey, const int lpDefaultValue, const char* const szPathAndFile, bool isWriteMode = false, int* lpReturnInt = 0 );

#endif //_ini_parser_h_