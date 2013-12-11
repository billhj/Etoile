/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Pose.cpp
* @brief 
* @date 1/2/2011
*/

#include "Pose.h"

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
	Pose::Pose()
	{

	}

	Pose::~Pose()
	{
		for(unsigned int i = 0; i < _offsets.size(); i++)
		{
			delete _offsets[i];
		}
		_offsets.clear();
	}

	void Pose::addOffset(int index, Vec3f xyz)
	{
		Offset* o = new Offset(index, xyz);
		_offsets.push_back(o);
	}
}