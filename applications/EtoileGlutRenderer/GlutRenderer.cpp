/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GlutRenderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "GlutRenderer.h"
#include "GlutWindow.h"

namespace Etoile
{
	GlutRenderer::GlutRenderer() : _pOutput(NULL)
	{
	}

	GlutRenderer::~GlutRenderer()
	{

	}

	void GlutRenderer::initGlut()
	{
		//glutInit (&argc, argv);
		glutInitWindowSize (_w, _h);
		glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutCreateWindow ("Glut Renderer");
	}

	void GlutRenderer::init()
	{
		glInfo* info = glInfo::getInstance();
		info->initAllOpenGL();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT,GL_FILL);
		//glFrontFace(GL_CW);
		glClearColor(0.0, 0.0, 0.0, 0.99);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		//glEnable(GL_MULTISAMPLE);	
	}

	void GlutRenderer::draw()
	{
		for(unsigned int i = 0; i < _renderPasses.size(); ++i)
		{
			_renderPasses[i]->draw();
		}
		if(_pOutput != NULL)
		{
			_pOutput->draw();
		}
	}

	void GlutRenderer::setRenderPasses(std::vector<RenderPass*> passes)
	{
		_renderPasses = passes;
	}

	void GlutRenderer::setOutputTexture(Texture * t)
	{
		_pOutput = t;
	}
}