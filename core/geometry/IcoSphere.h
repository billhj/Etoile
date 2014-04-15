/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IcoSphere.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef _ICOSPHERE_H
#define _ICOSPHERE_H

#include <vector>
#include "Primitive.h"

namespace Etoile
{
	class IcoSphere : public Primitive
	{
	  public:
		IcoSphere(unsigned int levels=1);
		virtual~IcoSphere(){}
		virtual void init();
	  protected:
		void subdivide();
		std::vector<std::vector<int>> _indices_tmp;
		std::vector<int> _listIds;
		int _level;
	};
}

#endif // _ICOSPHERE_H
