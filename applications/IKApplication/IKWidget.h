/**
* Copyright(C) 2009-2012                
* @author Jing HUANG 
* @file IKWidget.h
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
#include "animation/BVHLoader.h"
#include "geometry/AxisAlignedBoundingBox.h"
#include "Manipulator.h"
#include "animation/IK/MassSpringSolver.h"
#include "animation/IK/CCDSolver.h"
#include "animation/IK/JacobianDLSSolver.h"
#include "animation/IK/JacobianPseudoInverseSolver.h"
#include "animation/IK/JacobianTransposeSolver.h"
#include "animation/Frame.h"

#include <QTime>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

using namespace Etoile;

class IKWidget : public QGLViewer
{
	Q_OBJECT

		static const int WIDGETWIDTH = 800, WIDGETHEIGHT = 600;
public:
	int framesNb;
	std::vector<Frame> _states;
	IKWidget(QMainWindow* parent = 0): _pParent(parent), _scaleFactor(0.2), _selectedJointIndex(-1)
	{
		framesNb = 0;
	}

	~IKWidget(){}
	void initSkeleton()
	{
		_skeleton.clear();
		Joint* j = new Joint("0");
		j->setLocalPosition(Vec3f(0,0,0));
		_skeleton.addJoint(j, _skeleton.getJoints().size() - 1);
		j = new Joint("1");
		j->setLocalPosition(Vec3f(0,5,0));
		_skeleton.addJoint(j, _skeleton.getJoints().size() - 1);
		j = new Joint("2");
		j->setLocalPosition(Vec3f(0,5,0));
		_skeleton.addJoint(j, _skeleton.getJoints().size() - 1);
		j = new Joint("3");
		j->setLocalPosition(Vec3f(0,5,0));
		_skeleton.addJoint(j, _skeleton.getJoints().size() - 1);
		j = new Joint("4");
		j->setLocalPosition(Vec3f(0,5,0));
		_skeleton.addJoint(j, _skeleton.getJoints().size() - 1);
		j = new Joint("5");
		j->setLocalPosition(Vec3f(0,5,0));
		_skeleton.addJoint(j, _skeleton.getJoints().size() - 1);
		j = new Joint("6");
		j->setLocalPosition(Vec3f(0,5,0));
		_skeleton.addJoint(j, _skeleton.getJoints().size() - 1);
		_skeleton.update();
		//initBody();


	}

	void initBody()
	{
		_skeleton.clear();
		Joint* root = new Joint("root");
		root->setLocalPosition(Vec3f(0,5,0));
		_skeleton.addJoint(root, _skeleton.getJoints().size() - 1);

		Joint* body = new Joint("body");
		body->setLocalPosition(Vec3f(0,3,0));
		_skeleton.addJoint(body, root->getId());


		Joint* hipLeft = new Joint("hipLeft");
		hipLeft->setLocalPosition(Vec3f(-1, -1, 0));
		_skeleton.addJoint(hipLeft, root->getId());

		Joint* upperLegLeft = new Joint("upperLegLeft");
		upperLegLeft->setLocalPosition(Vec3f(0, -2, 0));
		_skeleton.addJoint(upperLegLeft, hipLeft->getId());

		Joint* downLegLeft = new Joint("downLegLeft");
		downLegLeft->setLocalPosition(Vec3f(0, -2, 0));
		_skeleton.addJoint(downLegLeft, upperLegLeft->getId());

		_skeleton.update();
		
		Joint* hipRight = new Joint("hipRight");
		hipRight->setLocalPosition(Vec3f(1, -1, 0));
		_skeleton.addJoint(hipRight, root->getId());

		Joint* upperLegRight = new Joint("upperLegRight");
		upperLegRight->setLocalPosition(Vec3f(0, -2, 0));
		_skeleton.addJoint(upperLegRight, hipRight->getId());

		Joint* downLegRight = new Joint("downLegRight");
		downLegRight->setLocalPosition(Vec3f(0, -2, 0));
		_skeleton.addJoint(downLegRight, upperLegRight->getId());
		
		_skeleton.update();
		double torqueR = 0.0;
	
		{
			Frame frame0;
			FrameParameters p;
			
			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			frame0.addFrameParameters("root", p);
			_skeleton.getJoint("root")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), -0.5);
			frame0.addFrameParameters("hipLeft", p);
			_skeleton.getJoint("hipLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 1);
			frame0.addFrameParameters("upperLegLeft", p);
			_skeleton.getJoint("upperLegLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), -0.1);
			frame0.addFrameParameters("hipRight", p);
			_skeleton.getJoint("hipRight")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.1);
			frame0.addFrameParameters("upperLegRight", p);
			_skeleton.getJoint("upperLegRight")->setLocalRotation(p._localRotation);
			
			_skeleton.update();
			Vec3f pos = _skeleton.getJoint("downLegRight")->getWorldPosition();
			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			p._translation = Vec3f(0, -pos.y(), pos.z()) + root->getWorldPosition();
			frame0.addFrameParameters("root", p);

			_states.push_back(frame0);
		
		}

		{
			Frame frame1;
			FrameParameters p;

			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			frame1.addFrameParameters("root", p);
			_skeleton.getJoint("root")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.5);
			frame1.addFrameParameters("hipLeft", p);
			_skeleton.getJoint("hipLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.1);
			frame1.addFrameParameters("upperLegLeft", p);
			_skeleton.getJoint("upperLegLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), -0.3);
			frame1.addFrameParameters("hipRight", p);
			_skeleton.getJoint("hipRight")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.1);
			frame1.addFrameParameters("upperLegRight", p);
			_skeleton.getJoint("upperLegRight")->setLocalRotation(p._localRotation);
			
			_skeleton.update();
			Vec3f pos = _skeleton.getJoint("downLegRight")->getWorldPosition();
			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			p._translation = Vec3f(0,-pos.y(),pos.z()) + root->getWorldPosition();
			frame1.addFrameParameters("root", p);

			_states.push_back(frame1);
		}
		
		{
			Frame frame2;
			FrameParameters p;

			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			frame2.addFrameParameters("root", p);
			_skeleton.getJoint("root")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), -0.1);
			frame2.addFrameParameters("hipLeft", p);
			_skeleton.getJoint("hipLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.1);
			frame2.addFrameParameters("upperLegLeft", p);
			_skeleton.getJoint("upperLegLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), -0.5);
			frame2.addFrameParameters("hipRight", p);
			_skeleton.getJoint("hipRight")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 1);
			frame2.addFrameParameters("upperLegRight", p);
			_skeleton.getJoint("upperLegRight")->setLocalRotation(p._localRotation);

			_skeleton.update();
			Vec3f pos = _skeleton.getJoint("downLegLeft")->getWorldPosition();
			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			p._translation = Vec3f(0,-pos.y(),pos.z()) + root->getWorldPosition();
			frame2.addFrameParameters("root", p);

			_states.push_back(frame2);
		}

		{
			Frame frame3;
			FrameParameters p;

			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			frame3.addFrameParameters("root", p);
			_skeleton.getJoint("root")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), -0.3);
			frame3.addFrameParameters("hipLeft", p);
			_skeleton.getJoint("hipLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.1);
			frame3.addFrameParameters("upperLegLeft", p);
			_skeleton.getJoint("upperLegLeft")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.5);
			frame3.addFrameParameters("hipRight", p);
			_skeleton.getJoint("hipRight")->setLocalRotation(p._localRotation);

			p._localRotation.setAxisAngle(Vec3f(1,0,0), 0.1);
			frame3.addFrameParameters("upperLegRight", p);
			_skeleton.getJoint("upperLegRight")->setLocalRotation(p._localRotation);
			
			_skeleton.update();
			Vec3f pos = _skeleton.getJoint("downLegLeft")->getWorldPosition();
			p._localRotation.setAxisAngle(Vec3f(1,0,0), torqueR);
			p._translation = Vec3f(0,-pos.y(),pos.z()) + root->getWorldPosition();
			frame3.addFrameParameters("root", p);

			_states.push_back(frame3);
		}
	}

	void drawSkeleton(Skeleton* skeleton)
	{
	/*	int nb = framesNb%(100);
		if(nb < 25)
		{
			updateFrame(_states[0], _states[1], nb /(float)25);
			//updateFrame(_states[0], _states[1], 0);
		}else if(nb < 50)
		{
			updateFrame(_states[1], _states[2], (nb - 25 )/(float)25);
			//updateFrame(_states[1], _states[2], 0);
		}else if(nb < 75)
		{
			updateFrame(_states[2], _states[3], (nb - 50 )/(float)25);
			//updateFrame(_states[2], _states[3], 0);
		}else if(nb < 100)
		{
			updateFrame(_states[3], _states[0], (nb - 75 )/(float)25);
			//updateFrame(_states[3], _states[0], 0);
		}
		framesNb++;*/
		for(int i = 0; i < int( _skeleton.getJoints().size()); ++i)
		{

			Joint* j = _skeleton.getJoint(i);
			if(j->getParent() != NULL)
			{
				Joint* jp = j->getParent();
			
				Vec3f posP = jp->getWorldPosition();
				Vec3f pos = j->getWorldPosition();
				float DiffuseMaterial0[3] = {0.5, 0.0, 0.0};          // define bones color
				glColor3fv(DiffuseMaterial0);
				glPointSize(10);
				glBegin(GL_POINTS);
				glVertex3d(posP.x(), posP.y(), posP.z());
				glEnd();
				float DiffuseMaterial[3] = {0.6, 0.3, 0.3};          // define bones color
				glColor3fv(DiffuseMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, &DiffuseMaterial[0]);
				_glDrawFunctions.drawBone(posP, pos, j->computeOriginalLocalPosition(Vec3f()), jp->getWorldRotation());
				drawCurrentAxis(posP, jp->getWorldRotation());
			}
		}
		if(_selectedJointIndex >= 0)
			drawInfo(_skeleton.getJoint(_selectedJointIndex));

	}

	void updateFrame(Frame f0, Frame f1, double i)
	{
		std::map<std::string, int>::iterator itor = f0.getIndices().begin();
		for(;itor != f0.getIndices().end(); ++itor)
		{
			std::string name = itor->first;
			Joint* j = _skeleton.getJoint(name);
			if(name == "root")
			{
				FrameParameters fp0 = f0.getFrameParameters()[itor->second];
				FrameParameters fp1 = f1.getFrameParameters()[itor->second];
				Vec3f pos = fp0._translation * (1 - i) + fp1._translation * i;
				j->setLocalPosition(pos);
			}
			FrameParameters fp0 = f0.getFrameParameters()[itor->second];
			FrameParameters fp1 = f1.getFrameParameters()[itor->second];
			Quaternionf q = Quaternionf::slerp(fp0._localRotation,fp1._localRotation,i);
			
			j->setLocalRotation(q);
			j->update();
		}
	}

	void updateSkeleton(Skeleton* skeleton)
	{
		skeleton->update();
	}

	void drawInfo(Joint* currentJoint)
	{
		if(currentJoint == NULL) return;

		const Vec3f& pos2 = currentJoint->getWorldPosition();
		{
		
			_glDrawFunctions.drawCurrentAxis(pos2.x(), pos2.y(), pos2.z(), currentJoint->getWorldRotation(), 1.0);
		
		}

		glLineWidth(0.3);
		Vec3f pos = currentJoint->getWorldPosition();
		glColor3f(1,0.4,0.7);
		qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(qglviewer::Vec(pos.x(), pos.y(), pos.z()));
		drawText((int)screenPos[0]+20, (int)screenPos[1]-10, QString(" %4: [ %1, %2, %3 ] ").arg(pos.x(), 0, 'f',2).arg(pos.y(), 0, 'f',2).arg(pos.z(), 0, 'f',2).arg(QString(currentJoint->getName().c_str())) );

	}

	void drawPlane()
	{
		glColor3f(0.8,0.8,0.8);
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

	void drawCurrentAxis(Vec3f pos, Quaternionf q)
	{
		glPushMatrix();
		double length = 5.0 *_scaleFactor;
		Vec3f xAxis(1,0,0);
		Vec3f yAxis(0,1,0);
		Vec3f zAxis(0,0,1);
		glLineWidth(3);
		Vec3f angle = q.getEulerAngleXYZBYAngle();
		glTranslatef(pos.x(),pos.y(),pos.z());
		xAxis = q * xAxis;
		yAxis = q * yAxis;
		zAxis = q * zAxis;
		/*glPushMatrix();
		glColor4f(1,0,0,1);
		glBegin(GL_LINES);
		glVertex3f(xAxis.x() * length , xAxis.y() * length , xAxis.z() * length);
		glVertex3f(0,0,0);
		glEnd();

		glColor4f(0,1,0,1);
		glBegin(GL_LINES);
		glVertex3f(yAxis.x() * length , yAxis.y() * length , yAxis.z() * length);
		glVertex3f(0,0,0);
		glEnd();

		glColor4f(0,0,1,1);
		glBegin(GL_LINES);
		glVertex3f(zAxis.x() * length , zAxis.y() * length ,zAxis.z() * length);
		glVertex3f(0,0,0);
		glEnd();
		glPopMatrix();*/

		glRotatef(angle.z(), 0, 0, 1); 
		glRotatef(angle.y(), 0, 1, 0);
		glRotatef(angle.x(), 1, 0, 0);
		drawAxis(length);
		glPopMatrix();
	}
	void drawWireFrame(){}
	void releaseResource(){}

	void setIKSolver(IKSolver* solver)
	{
		_pSolver = solver;
	}

	IKSolver* getIKSolver(){return _pSolver;}

protected:
	void keyPressEvent(QKeyEvent *e)
	{
		if (e->key() == Qt::Key_F5)
		{
			initSkeleton();
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
			Quaternionf q = _manipulator.getCurrentRotation();
			Joint* j = _skeleton.getJoint(_selectedJointIndex);

			_pSolver = new MassSpringSolver();
			_pSolver->compute(_skeleton.getJoints(), Vec3f(_manipulator.getPosition().x, _manipulator.getPosition().y, _manipulator.getPosition().z), false);

		}
		else if(e->key() == Qt::Key_F2)
		{
		}
		else if(e->key() == Qt::Key_F3)
		{
		}else if(e->key() == Qt::Key_F4)
		{
		}else if(e->key() == Qt::Key_Up)
		{
		}else if(e->key() == Qt::Key_Down)
		{
		}
		else if(e->key() == Qt::Key_Space)
		{
			_pause = !_pause;
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
			QMenu menu( this );
			std::map<QAction*, int> menuMap;
			menuMap[menu.addAction("MassSpring")] = 1;
			menuMap[menu.addAction("CCD")] = 2;
			menuMap[menu.addAction("PseudoInverse")] = 3;
			menuMap[menu.addAction("Transpose")] = 4;
			menuMap[menu.addAction("DLS")] = 5;

			menu.setMouseTracking(true);
			QAction* action = menu.exec(event->globalPos());
			if(action!= NULL)
			{
				int id = (menuMap.find(action)->second);
				int maxnb_iterations = _pSolver->getMaxNumberOfTries();
				if(id == 1)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::MassSpringSolver(maxnb_iterations);
				}
				else if(id == 2)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::CCDSolver(maxnb_iterations);
				}
				else if(id == 3)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianPseudoInverseSolver(maxnb_iterations);
				}
				else if(id == 4)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianTransposeSolver(maxnb_iterations);
				}
				else if(id == 5)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianDLSSolver(maxnb_iterations);
				}else
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::MassSpringSolver(maxnb_iterations);
				}
				std::cout<<"now using : " <<_pSolver->getIKSolverName() <<std::endl;
			}
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
			Vec3f posM;
			const Etoile::Joints& joints = _skeleton.getJoints();
			Etoile::Joints::const_iterator itor = joints.begin();
			for(;itor != joints.end(); ++itor, ++i)
			{
				Joint* joint = (*itor);
				Vec3f pos = joint->getWorldPosition();
				qglviewer::Vec screenPos = cam->projectedCoordinatesOf(qglviewer::Vec(pos[0],pos[1],pos[2]));
				double dis = Vec2d(   Vec2d(screenPos[0] , screenPos[1]) - Vec2d(x , y)   ).length();
				if( dis < minScreenDistance )
				{
					minScreenDistance = dis;
					_selectedJointIndex = i;
					name = joint->getName();
					posM = pos;
				}
			}
			std::cout<<"selected Joint: " << name<<std::endl;
			_manipulator.reset();
			_manipulator.setOrigine(qglviewer::Vec(posM.x(), posM.y(), posM.z()));

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

		drawPlane();
		drawSkeleton(&_skeleton);
		printOpenGLError();
		//drawAxis();
		glColor3f(0.9,0.25,0.55);
		QFont serifFont("Times", 10, QFont::Bold);
		drawText((int)30, (int)25, QString(" shift + right_mouse : choose one joint !"), serifFont);

		glPushMatrix();
		_manipulator.draw();
		glPopMatrix();

		if(_pSolver != NULL)
		{
			drawText((int)600, (int)25, QString(_pSolver->getIKSolverName().c_str()), serifFont);
			drawText((int)600, (int)40, QString("Max Iterations: %1").arg(_pSolver->getMaxNumberOfTries()), serifFont);
			drawText((int)600, (int)55, QString("Distance Threshold: %1").arg(_pSolver->getTargetThreshold()), serifFont);
		}
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
		connect(&_manipulator, SIGNAL(mouseReleased()), this, SLOT(applyJoint()));

		//_solvers.push_back(new MassSpringSolver());
		//_solvers.push_back(new CCDSolver());
		//_solvers.push_back(new JacobianDLSSolver());
		//_solvers.push_back(new JacobianTransposeSolver());
		//_solvers.push_back(new JacobianPseudoInverseSolver());
		_pSolver = new MassSpringSolver();
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
			Quaternionf q = _manipulator.getCurrentRotation();
			Joint* j = _skeleton.getJoint(_selectedJointIndex);
			if((Vec3f(_manipulator.getPosition().x, _manipulator.getPosition().y, _manipulator.getPosition().z) -  j->getWorldPosition()).length() < 0.1)
			{
				j->setLocalRotation(q);
				j->update();
			}
			else
			{
				QTime myTimer;
				myTimer.start();
				if(_pSolver != NULL)
				{
					_pSolver->compute(_skeleton.getJoints(), Vec3f(_manipulator.getPosition().x, _manipulator.getPosition().y, _manipulator.getPosition().z), false);
					int nMilliseconds = myTimer.elapsed();
					std::cout<<_pSolver->getIKSolverName() << " nMilliseconds : " << nMilliseconds<<std::endl;
				}
			}
		}
public:

	int _selectedJointIndex;
	bool _useConstraints;
	int _frameIdx;

	bool _pause;
	bool _cycle;
	unsigned int _fps;
	AxisAlignedBoundingBoxf _boundingBox;
	double _scaleFactor;

	glDrawFunctions _glDrawFunctions;

	Skeleton _skeleton;
	SimpleManipulator _manipulator;
	IKSolver* _pSolver;
	//std::vector<IKSolver*> _solvers;
};
