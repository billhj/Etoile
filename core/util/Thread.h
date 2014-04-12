/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Thread.h
* @brief 
* @date 1/2/2011
*/

#pragma once

/*
#include <QThread> 
#include <QMutex>
#include "../Others/Macros.h"*/

namespace Etoile
{

	class Mutex //: public QMutex
	{

	};

	class Thread //: public QThread
	{
	public:
		Thread()
		{
		}

		virtual void run(); //MACRO_OVERRIDE = 0;

	private:

	};
}


