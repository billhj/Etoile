/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLQuadRenderPass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLRenderPass.h"
#include "GLSLGpuProgram.h"
#include "geometry/Material.h"
namespace Etoile
{

	class GLQuadRenderPass : public GLRenderPass
	{
		//Quad* quad;
	protected:
		Material* _pmaterial;
	public:
		GLQuadRenderPass(const std::string& name): GLRenderPass(name)
		{
			_pmaterial = new Material("Quad");
		}

		virtual void draw() override
		{
			useRenderTargets();
			if(_pmaterial != NULL)
			{
				GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)_pmaterial->getGpuProgram();
				if(gpuprogram != NULL)
				{
					std::map<std::string, Texture*>& idxs = _pmaterial->getTextures();
					std::map<std::string, Texture*>::iterator itor;

					for(itor = idxs.begin(); itor != idxs.end(); ++itor)
					{
						std::string bName = itor->first;
						Texture* t = itor->second;
						gpuprogram->bindTexture(bName, t);
					}

					gpuprogram->use();
					printOpenGLError();
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
					gpuprogram->unUse();
					gpuprogram->unBindBindingTextures();
				}
			}
			unUseRenderTargets();
		}
		Material* getMaterial(){return _pmaterial;}
		void setMaterial(Material* material){_pmaterial = material;}
	};
}
