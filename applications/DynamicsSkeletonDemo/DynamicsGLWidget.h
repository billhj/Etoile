/**
* Copyright(C) 2009-2012                
* @author Jing HUANG 
* @file DynamicsGLWidget.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/glhead.h"
#include <QGLViewer/qglviewer.h>
#include "geometry/BipedSkeleton.h"
#include <QMainWindow>
#include <QString>
#include <QTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSize>
#include <QTimer>
#include "renderer/OpenGL/glDrawFunctions.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


class DynamicsGLWidget : public QGLViewer
{
	Q_OBJECT
		static const int WIDGETWIDTH = 800, WIDGETHEIGHT = 600;
public:
	DynamicsGLWidget(QMainWindow* parent = 0): _pParent(parent), _selectedJointIndex(-1)
	{
	}

	~DynamicsGLWidget(){}
	void initSkeleton()
	{
		
	}

	void drawSkeleton(Etoile::Skeleton* skeleton)
	{
		for(int i = 0; i < int( _skeleton->getJoints().size()); ++i)
		{

			Etoile::Joint* j = _skeleton->getJoint(i);
			if(j->getParent() != NULL)
			{
				Etoile::Joint* jp = j->getParent();
			
				Etoile::Vec3f posP = jp->getWorldPosition();
				Etoile::Vec3f pos = j->getWorldPosition();
				float DiffuseMaterial0[3] = {0.5, 0.0, 0.0};          // define bones color
				glColor3fv(DiffuseMaterial0);
				glPointSize(10);
				glBegin(GL_POINTS);
				glVertex3d(posP.x(), posP.y(), posP.z());
				glEnd();
				float DiffuseMaterial[3] = {0.6, 0.3, 0.3};          // define bones color
				glColor3fv(DiffuseMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, &DiffuseMaterial[0]);
				_glDrawFunctions.drawBone(posP, pos, j->getLocalPosition(), jp->getWorldRotation());
				_glDrawFunctions.drawCurrentAxis(posP.x(), posP.y(), posP.z(), jp->getWorldRotation(), 1);
			}
		}
		if(_selectedJointIndex >= 0)
			drawInfo(_skeleton->getJoint(_selectedJointIndex));
	}


	void drawInfo(Etoile::Joint* currentJoint)
	{
		if(currentJoint == NULL) return;

		glLineWidth(0.3);
		Etoile::Vec3f pos = currentJoint->getWorldPosition();
		glColor3f(1,0.4,0.7);
		qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(qglviewer::Vec(pos.x(), pos.y(), pos.z()));
		drawText((int)screenPos[0]+20, (int)screenPos[1]-10, QString(" %4: [ %1, %2, %3 ] ").arg(pos.x(), 0, 'f',2).arg(pos.y(), 0, 'f',2).arg(pos.z(), 0, 'f',2).arg(QString(currentJoint->getName().c_str())) );

	}

	void drawPlane()
	{
		glColor3f(0.0,0.8,0.8);
		glPushMatrix();
		glBegin(GL_QUADS);
		glNormal3d(0,1,0);
		/*glVertex3f(5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,50000);
		glVertex3f(5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,-50000);
		glVertex3f(-5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,-50000);
		glVertex3f(-5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,50000);*/
		glVertex3f(5000,0,50000);
		glVertex3f(5000,0,-50000);
		glVertex3f(-5000,0,-50000);
		glVertex3f(-5000,0,50000);
		glEnd();
		glPopMatrix();
	}


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
			
		}
		else if(e->key() == Qt::Key_F2)
		{
			//body->update(.10f);
		}
		else if(e->key() == Qt::Key_F3)
		{
		
			//body->addForce(Eigen::Vector3f(-10,0,0));

		}else if(e->key() == Qt::Key_F4)
		{
			//body->addTorque(Eigen::Vector3f(0,1,0));
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

			// select one mass
			qglviewer::Camera *cam = this->camera();
			int x = event->x();
			int y = event->y();
			float minScreenDistance = 10000;

			int i = 0;
			std::string name;
			Etoile::Vec3f posM;
			const Etoile::Joints& joints = _skeleton->getJoints();
			Etoile::Joints::const_iterator itor = joints.begin();
			for(;itor != joints.end(); ++itor, ++i)
			{
				Etoile::Joint* joint = (*itor);
				Etoile::Vec3f pos = joint->getWorldPosition();
				qglviewer::Vec screenPos = cam->projectedCoordinatesOf(qglviewer::Vec(pos[0],pos[1],pos[2]));
				double dis = Etoile::Vec2d(   Etoile::Vec2d(screenPos[0] , screenPos[1]) - Etoile::Vec2d(x , y)   ).length();
				if( dis < minScreenDistance )
				{
					minScreenDistance = dis;
					_selectedJointIndex = i;
					name = joint->getName();
					posM = pos;
				}
			}
			std::cout<<"selected Joint: " << name<<std::endl;
			
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
		GLfloat light_position0[] = { 0, 30, 50, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

		GLfloat light_position1[] = { 0, 30, -50, 1.0 };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

		printOpenGLError();

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		printOpenGLError();

		//drawBox();
		drawPlane();
		drawSkeleton(_skeleton);
		printOpenGLError();
		//drawAxis();
		glColor3f(0.9,0.25,0.55);
		QFont serifFont("Times", 10, QFont::Bold);
		drawText((int)30, (int)25, QString(" shift + right_mouse : choose one joint !"), serifFont);
	}

	virtual void init()
	{
		setMouseTracking(true);
		setFocusPolicy(Qt::StrongFocus);
		setShortcut(DISPLAY_FPS,Qt::CTRL+Qt::Key_F);

		this->setFixedSize(WIDGETWIDTH, WIDGETHEIGHT );
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT,GL_FILL);
		//glDisable(GL_COLOR_MATERIAL);
		glClearColor(0.8, 0.8, 0.8, 0.99);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_MULTISAMPLE);

		qglviewer::Camera *cam = this->camera();
		cam->setSceneCenter(qglviewer::Vec(0,0,0));
		cam->setSceneRadius(50);
		cam->setPosition(qglviewer::Vec(0,5,20));

		this->setMouseTracking(true);

		setHandlerKeyboardModifiers(QGLViewer::FRAME, Qt::AltModifier);
		setHandlerKeyboardModifiers(QGLViewer::CAMERA, Qt::NoModifier);
		setHandlerKeyboardModifiers(QGLViewer::FRAME, Qt::ControlModifier);

		setAnimationPeriod(40);
		startAnimation();

		GLfloat light_ambient0[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };

		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

		GLfloat light_ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };

		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

		initSkeleton();

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
		void applyJoint()
		{
			
		}
public:

	int _selectedJointIndex;
	bool _pause;
	bool _cycle;
	unsigned int _fps;

	Etoile::Skeleton* _skeleton;
	Etoile::glDrawFunctions _glDrawFunctions;
};
