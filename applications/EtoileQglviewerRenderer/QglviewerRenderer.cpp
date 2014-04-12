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
#include "renderer/OpenGL/GLTexture2D.h"

namespace Etoile
{
	QglviewerRenderer::QglviewerRenderer(QglviewerPlugin* plugin) : QGLViewer(), _pOutput(NULL), _pParent(plugin), _pbackgroud(NULL)
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
		this->setSceneRadius(20);
		showEntireScene();
		glDisable(GL_COLOR_MATERIAL);
		//glEnable(GL_MULTISAMPLE);	
		setAnimationPeriod(15);
		startAnimation();

		initBackgroundTexture();
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
			
			//// set the blending mode
			//		
			
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA); 
			//glDisable( GL_DEPTH_TEST );
			backgrounddraw();
			_pOutput->draw(this->width(), this->height());
			//glEnable( GL_DEPTH_TEST );
	
		}else
		{
			backgrounddraw();
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


	void QglviewerRenderer::initBackgroundTexture()
	{
		QImage qimage;
		QImage qtexture;
		bool b = qimage.load("./img/bg.png");
		Etoile::GLTexture2D* t = new Etoile::GLTexture2D("background");
		qtexture = QGLWidget::convertToGLFormat(qimage);
		t->create(qimage.width(), qimage.height(),1 , 0x8814, GL_RGBA, GL_UNSIGNED_BYTE, ((float*)(qtexture.bits())), false);

		float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
		Etoile::GLTexture2D* t2 = new Etoile::GLTexture2D("checkBoardMap");
		t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);
		if(b)
			this->setbackgroundTexture(t);
		else
			this->setbackgroundTexture(t2);
	}

	void QglviewerRenderer::backgrounddraw()
	{

		if(_pbackgroud==NULL) return;
		//glDepthMask(false);
		_pbackgroud->use();
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
		_pbackgroud->unUse();
		//glDepthMask(true);
	
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