/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Log.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <sstream>

namespace Etoile
{
	class Log : public std::ostringstream
	{
	public:
		Log(void);
		~Log(void);
		Log& debug();
		Log& debugfull();
		/*std::ostringstream& operator<< (float value){
		*this<<value;
		return _os; 
		}

		std::ostringstream& operator<< (const std::string& value){
		*this<<value;
		return _os; 
		}*/

	};

#if ( defined( _DEBUG ) || defined( DEBUG ) )
#define DEBUGLOGER  Log().debug()
#else
#define DEBUGLOGER Log().debug()
#endif

}