/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @Dir Dir.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
#include <direct.h>
#define getCurrentDir _getcwd
#else
#include <unistd.h>
#define getCurrentDir getcwd
#endif

namespace Etoile
{

	class Dir
	{
	public:
		/// This should be left the only constructor
		Dir(){}
		virtual ~Dir(){}
		// static methods
		static const std::string getCurrentDirectory()
		{
			char cCurrentPath[FILENAME_MAX];
			if (getCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
			{
				cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
				return std::string(cCurrentPath);
			}
			return "";
		}

	private:

	};


}

