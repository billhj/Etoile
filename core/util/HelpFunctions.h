/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HelpFunctions.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <algorithm> 
#include <string>  
#include <iostream>
#include <cctype>
#include <sstream>

namespace Etoile
{
	void ToLowerCaseSTD(std::string &str)
	{

#if (defined _WIN32 || defined _WIN64 )
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
#else
		/*const int length = str.length();
		for(int i=0; i < length; ++i)
		{
		str[i] = std::tolower(str[i]);
		}*/
		std::transform(str.begin(), str.end(), str.begin(), std::ptr_fun<int,int>(std::tolower));
#endif

	}

	void ToUpperCaseSTD(std::string &str)
	{
#if (defined _WIN32 || defined _WIN64 )
		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
#else
		/*const int length = str.length();
		for(int i=0; i < length; ++i)
		{
		str[i] = std::toupper(str[i]);
		}*/
		std::transform(str.begin(), str.end(), str.begin(), std::ptr_fun<int,int>(std::toupper));
#endif
	}


	std::string getStringFromValue(float value)
	{
		std::string s;
		std::stringstream ss;
		ss << value;
		s = ss.str();
		return s;
	}


	void trimString( std::string& _string) 
	{
		// Trim Both leading and trailing spaces
		size_t start = _string.find_first_not_of(" \t\r\n");
		size_t end   = _string.find_last_not_of(" \t\r\n");

		if(( std::string::npos == start ) || ( std::string::npos == end))
			_string = "";
		else
			_string = _string.substr( start, end-start+1 );
	}
}

