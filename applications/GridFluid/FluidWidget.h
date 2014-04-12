/**
* Copyright(C) 2009-2012                
* @author Jing HUANG 
* @file FluidWidget.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glDrawFunctions.h"
#include <QGLViewer/qglviewer.h>
#include <QMainWindow>
#include <QString>
#include <QTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSize>
#include <QTimer>
#include "geometry/AxisAlignedBoundingBox.h"
#include "animation/fluid/GridFluid.h"
#include <QTime>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

using namespace Etoile;

class FluidWidget : public QGLViewer
{
	Q_OBJECT

		static const int WIDGETWIDTH = 800, WIDGETHEIGHT = 600;
public:
	FluidWidget(QMainWindow* parent = 0): _pParent(parent)
	{
	}

	~FluidWidget(){}



	void releaseResource(){}


protected:
	void keyPressEvent(QKeyEvent *e)
	{
		if (e->key() == Qt::Key_F5)
		{
		}
		else if (e->key() == Qt::Key_F6)
		{
		}
		else if(e->key() == Qt::Key_F9)
		{
			std::cout<<"widget size:"<<this->size().width()<<this->size().height()<<std::endl;
		}
		else if(e->key() == Qt::Key_F1)
		{
			_gridFluid.addSourceDensity(Vec3i(2,2,2), 800);

		}
		else if(e->key() == Qt::Key_F2)
		{
			_gridFluid.addForceVelocity(Vec3i(2,2,2), Vec3d(0,5,0));
			
		}
		else if(e->key() == Qt::Key_F3)
		{
			_gridFluid.update();
			std::cout<<"update"<<std::endl;
		}
		else if(e->key() == Qt::Key_F4)
		{
		}else if(e->key() == Qt::Key_Up)
		{
		}else if(e->key() == Qt::Key_Down)
		{
		}
		else if(e->key() == Qt::Key_Space)
		{
		}
		else
			QGLViewer::keyPressEvent(e);
	}

	void mouseMoveEvent(QMouseEvent* event)
	{
		if ( (event->modifiers() & Qt::ShiftModifier) )
		{
		}
		else
			QGLViewer::mouseMoveEvent(event);
	}

	void mousePressEvent(QMouseEvent* event)
	{
		if ((event->button() == Qt::MiddleButton))
		{
		}
		else if ((event->modifiers() & Qt::ShiftModifier))
		{
		}
		else
			QGLViewer::mousePressEvent(event);
	}

	void mouseReleaseEvent(QMouseEvent* event)
	{
		QGLViewer::mouseReleaseEvent(event);
	}

	void wheelEvent ( QWheelEvent * event )
	{
		if (event->modifiers() & Qt::ShiftModifier)
		{

		}
		else
			QGLViewer::wheelEvent(event);
	}

private:
	virtual void draw()
	{
		/*GLfloat light_position0[] = { 0, 30, 50, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

		GLfloat light_position1[] = { 0, 30, -50, 1.0 };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);*/

		printOpenGLError();

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		printOpenGLError();
		//drawVelocity();
		drawDensity();
	}

	void drawVelocity()
	{
		Vec3i size = _gridFluid.getSize();
		glColor3f ( 1.0f, 1.0f, 1.0f );
		glLineWidth ( 15.0f );
		glBegin ( GL_LINES );
		for(unsigned int i = 1; i < size.x(); ++i)
		{
			for(unsigned int j = 1; j < size.y(); ++j)
			{
				for(unsigned int k = 1; k < size.z(); ++k)
				{
					glVertex3f (i, j, k);
					glVertex3f (i+_gridFluid.getFluidUnit(i,j,k)._velocity.x(), j+_gridFluid.getFluidUnit(i,j,k)._velocity.y(), k+_gridFluid.getFluidUnit(i,j,k)._velocity.z());
				}
			}
		}
		glEnd ();	
	}

	void drawDensity()
	{
		Vec3i size = _gridFluid.getSize();
		glPointSize(20);
		glBegin(GL_POINTS);
		for(unsigned int i = 1; i < size.x(); ++i)
		{
			for(unsigned int j = 1; j < size.y(); ++j)
			{
				for(unsigned int k = 1; k < size.z(); ++k)
				{
					glColor4f(0.2 * _gridFluid.getFluidUnit(i,j,k)._density, 0.2 * _gridFluid.getFluidUnit(i,j,k)._density, 0.1 * _gridFluid.getFluidUnit(i,j,k)._density, 0.1);
					//glColor4f(0.3, 0.2, 0.1, 0.05);
					glVertex3d(i, j, k);
					//std::cout<<i<<"  " <<j<< " "<<k<<"  :  "<<_gridFluid.getFluidUnit(i,j,k)._density<<std::endl;
				}
			}
		}
		glEnd();
	}

	virtual void init()
	{
		setMouseTracking(true);
		setFocusPolicy(Qt::StrongFocus);
		setShortcut(DISPLAY_FPS,Qt::CTRL+Qt::Key_F);

		this->setFixedSize(WIDGETWIDTH, WIDGETHEIGHT );
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	/*	glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT,GL_FILL);*/
		glEnable(GL_COLOR_MATERIAL);
		glClearColor(0, 0, 0, 1);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_MULTISAMPLE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);


		qglviewer::Camera *cam = this->camera();
		cam->setSceneCenter(qglviewer::Vec(0,0,0));
		cam->setSceneRadius(50);
		cam->setPosition(qglviewer::Vec(0,5,20));

		this->setMouseTracking(true);

		setHandlerKeyboardModifiers(QGLViewer::FRAME, Qt::AltModifier);
		setHandlerKeyboardModifiers(QGLViewer::CAMERA, Qt::NoModifier);
		setHandlerKeyboardModifiers(QGLViewer::FRAME, Qt::ControlModifier);

		setAnimationPeriod(100);
		startAnimation();

		GLfloat light_ambient0[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };

		glDisable(GL_LIGHTING);
		/*glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

		GLfloat light_ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };

		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);*/
		_gridFluid.buildGrid(10, 10, 10);
	}

	virtual QSize sizeHint () const
	{
		return QSize(WIDGETWIDTH,WIDGETHEIGHT);
	}
	virtual QSize minimumSizeHint () const
	{
		return QSize(WIDGETWIDTH,WIDGETHEIGHT);
	}
	virtual QSize maxmumSizeHint () const
	{
		return QSize(WIDGETWIDTH,WIDGETHEIGHT);
	}


	QMainWindow* _pParent;

signals:

	public slots:

public:
	GridFluid _gridFluid;
	AxisAlignedBoundingBox _boundingBox;
	glDrawFunctions _glDrawFunctions;
};
