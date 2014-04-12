/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLRenderPass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/RenderPass.h"
#include "glhead.h"
#include "FBOManager.h"
#include "GLTexture.h"

namespace Etoile
{

	class GLRenderPass : public RenderPass
	{
	protected:
		int _w, _h;
		int _x, _y;
		FBO* _pfbo;
	public:
		GLRenderPass(const std::string& name): RenderPass(name), _pfbo(NULL)
		{
			_w = 800, _h = 600;
			_x = 0, _y = 0;
			initFBO();
		}
		~GLRenderPass(){ if(_pfbo != NULL) delete _pfbo;}
		void setViewPort(int x, int y, int w, int h)
		{
			initFBO(w, h);
			_w = w, _h = h;
			_x = x, _y = y;
		}

		void initFBO()
		{
			if(_pfbo != NULL) delete _pfbo;
			_pfbo = new FBO();
			_pfbo->initialize(_w,_h);
		}

		void initFBO(int w, int h)
		{
			if(_pfbo == NULL)
			{
				_pfbo = new FBO();
				_pfbo->initialize(w, h);
			}
			else if(_pfbo->getWidth() != w || _pfbo->getHeight() != h)
			{
				if(_pfbo != NULL) delete _pfbo;
				_pfbo = new FBO();
				_pfbo->initialize(w, h);
			}
			
		}
		
		virtual void draw() = 0;

		virtual void useRenderTargets() override
		{
			glPushAttrib(GL_VIEWPORT_BIT);
			glViewport(_x, _y, _w, _h);
			if(_outputs.size() > 0)
			{
				_pfbo->beginTextureBinding();
				for(unsigned int i = 0; i < this->_outputs.size(); ++i)
				{
					TextureRenderTarget* rt =  dynamic_cast<TextureRenderTarget*>(_outputs[i]);
					if(rt != NULL)
					{
						GLTexture* t = (GLTexture*)(rt->get());
						if(t != NULL)
						{
							_pfbo->textureBinding(t->getId());
						}
					}
				}
				_pfbo->endTextureBinding();
			}
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		}

		virtual void unUseRenderTargets() override
		{
			if(_outputs.size() > 0)
			{
				_pfbo->unbindTexture2D(_outputs.size());
			}
			glPopAttrib();
			printOpenGLError();
		}
	};
}
