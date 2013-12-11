/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Log.cpp
* @brief 
* @date 1/2/2011
*/

#include "Log.h"
#include <time.h>

namespace Etoile
{
	Log::Log(void)
	{

	}


	Log::~Log(void)
	{
		(*this) << std::endl;
		fprintf(stderr, "%s", this->str().c_str());
		fflush(stderr);
	}


	Log& Log::debug()
	{
		(*this) <<"[debug]: ";
		return (*this);
	}

	Log& Log::debugfull()
	{
		time_t rawtime;
		time (&rawtime );
		struct tm * timeinfo;
		char buffer [80];
		timeinfo = localtime ( &rawtime );
		strftime (buffer,80," %A %B %d %Y %X %S \n",timeinfo);
		(*this) << "- "<< buffer<< " debug :";
		return (*this);
	}

}