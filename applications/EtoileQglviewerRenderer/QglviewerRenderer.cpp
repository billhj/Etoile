/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QglviewerRenderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "QglviewerRenderer.h"
#include "QglviewerPlugin.h"
#include <QIcon>
namespace Etoile
{
	QglviewerRenderer::QglviewerRenderer(QglviewerPlugin* plugin) : QGLViewer(), _pOutput(NULL), _pParent(plugin)
	{
		this->setWindowTitle("Qglviewer");
		setWindowIcon(QIcon("./styles/icon.png"));
	}

	QglviewerRenderer::~QglviewerRenderer()
	{

	}

	void QglviewerRenderer::init()
	{
		glInfo* info = glInfo::getInstance();
		info->initAllOpenGL();
		setMouseTracking(true);
		setFocusPolicy(Qt::StrongFocus);
		setShortcut(DISPLAY_FPS,Qt::CTRL+Qt::Key_F);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT,GL_FILL);
		//glFrontFace(GL_CW);
		glClearColor(0.0,0.0,0.0,1);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		//glEnable(GL_MULTISAMPLE);	
		//setAnimationPeriod();
		startAnimation();
	}

	void QglviewerRenderer::draw()
	{

		if(_pOutput != NULL)
		{
			updateMatrix();
			for(unsigned int i = 0; i < _renderPasses.size(); ++i)
			{
				_renderPasses[i]->draw();
			}
			_pOutput->draw(this->width(), this->height());
		}
	}

	void QglviewerRenderer::setRenderPasses(std::vector<RenderPass*> passes)
	{
		_renderPasses = passes;
	}

	void QglviewerRenderer::setOutputTexture(Texture * t)
	{
		_pOutput = t;
		if(t != NULL)
		{
			this->resize(t->getWidth(), t->getHeight());
		}
	}

	void QglviewerRenderer::updateMatrix()
	{
		if(_pParent != NULL)
		{
			Matrix4f view, proj;
			/*GLdouble m[4][4], p[4][4];
			this->camera()->getProjectionMatrix(m[0]);
			this->camera()->getModelViewMatrix(p[0]);
			for(int y = 0; y < 4; ++y)
			{
				for(int x = 0; x < 4; ++x)
				{
					view[y][x] = m[y][x];
					proj[y][x] = p[y][x];
				}
			}*/

			glMatrixMode (GL_PROJECTION);
			glGetFloatv(GL_PROJECTION_MATRIX , &(proj[0][0]));
			printOpenGLError ();
			glMatrixMode(GL_MODELVIEW);
			glGetFloatv(GL_MODELVIEW_MATRIX ,&(view[0][0]));

			_pParent->updateModelViewMatrix(&view, &proj);
		}
	}
}