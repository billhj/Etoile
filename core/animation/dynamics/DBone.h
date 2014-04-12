/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DBone.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <Eigen/Dense>
#include "DRigidBody.h"

namespace Etoile
{
	
	class DBone : public DRigidBody
	{
	public:
		DBone(const std::string& name): DRigidBody(name)
		{
		}

	protected:
		
	};

	
}