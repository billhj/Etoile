/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionEdge.cpp
* @brief 
* @date 1/2/2011
*/

#include "MotionEdge.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	MotionEdge::MotionEdge() : GraphEdge()
	{

	}

	MotionEdge::~MotionEdge()
	{

	}

}
