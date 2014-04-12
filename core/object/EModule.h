/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EModule.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "EObject.h"
#include "EReceptor.h"
#include "EEmitter.h"

namespace Etoile
{
	class EModule : public EObject, public EEmitter, public EReceptor
	{
	public:
		EModule(const std::string& name) : EObject(), EEmitter(), EReceptor(), _name(name)
		{
			_className = "EModule";
		}

		virtual std::string initClassName() = 0;

		std::string getModuleName(){return _name;}
		void setModuleName(const std::string& name){_name = name;}
	protected:
		std::string _name;
	};

}