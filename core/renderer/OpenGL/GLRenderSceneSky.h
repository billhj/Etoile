/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Scene.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <sstream>
#include "geometry/SceneSky.h"
#include "GLTexture2D.h"

namespace Etoile
{
	class GLRenderSceneSky : public SceneSky
	{
	public:
		GLRenderSceneSky() : SceneSky()
		{
			GLTexture2D* _pCheckBoard;
			GLTexture2D* _pWhite;
			GLTexture2D* _pBlack;
			float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
			_pCheckBoard = new Etoile::GLTexture2D("checkBoardMap");
			_pCheckBoard->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);

			float whiteMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
			_pWhite = new Etoile::GLTexture2D("whiteMap");
			_pWhite->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &whiteMap[0], false);
			float blackMap[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
			_pBlack = new Etoile::GLTexture2D("blackMap");
			_pBlack->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &blackMap[0], false);
			
			idx["checkBoardMap"] = idx.size();
			_skyList.push_back(_pCheckBoard);
			idx["whiteMap"] = idx.size();
			_skyList.push_back(_pWhite);
			idx["blackMap"] = idx.size();
			_skyList.push_back(_pBlack);
		}
	
		virtual void perform() 
		{
			glDepthMask(false);
			_pSky->use();
			glMatrixMode (GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity ();
			glMatrixMode (GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
			glEnd();

			glMatrixMode (GL_PROJECTION);
			glPopMatrix();
			glMatrixMode (GL_MODELVIEW);
			glPopMatrix();
			_pSky->unUse();
			glDepthMask(true);
		}
	protected:
	
	};

}