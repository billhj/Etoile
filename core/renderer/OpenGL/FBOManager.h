/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file FBOManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "FBO.h"

namespace Etoile
{
	class FBOManager
	{
	private:
		FBOManager();
	public:
		~FBOManager();
		FBO* getFBO(int width, int height);
		static FBOManager* getInstance(){
			static FBOManager fbomana;
			return &fbomana;
		}
		void clear();

	private:
		std::vector<FBO*> _fbolist;

	};
}
