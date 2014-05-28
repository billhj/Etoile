/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MocapWidget.cpp
* @brief 
* @date 1/2/2011
*/

#include "MocapWidget.h"
#include <QMenu>
#include <QLabel>
#include <QFileDialog>
#include "renderer/OpenGL/glDrawFunctions.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

using namespace Etoile;
MocapWidget::MocapWidget(QMainWindow* parent): QGLViewer(), _pParent(parent) , _selectedJointIndex(-1), _frameIdx(0)
{
	//    QGLFormat newFormat(this->context()->format());
	//    newFormat.setDoubleBuffer(true);
	//    newFormat.setVersion(5,1);
	//    setFormat(newFormat);

	_maxnb_iterations = 50;
	_useConstraints = false;
	_pause = false;
	_cycle = false;
	_fps = 25;
	_scaleFactor = 1;
}



MocapWidget::~MocapWidget()
{

}

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

void MocapWidget::init()
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
	glClearColor(0.7, 0.8, 0.7, 0.1);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_MULTISAMPLE);

	qglviewer::Camera *cam = this->camera();
	cam->setSceneCenter(qglviewer::Vec(0,0,0));
	cam->setSceneRadius(10);
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

	//initFramework();
	connect(&_timer, SIGNAL(timeout()), this, SLOT(updateFrameByTimer()));
}


void MocapWidget::openBVH()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Animation (*.bvh)"));
	initFramework(fileName.toStdString());
}

void MocapWidget::initFramework(const std::string& name)
{
	Skeleton* skeleton = new Skeleton(name);
	KeyFrameSequence* sequence = new KeyFrameSequence(name);
	bool b = _bvhloader.loadFromBVHFile(name, sequence, skeleton);
	std::vector<Vec3f> poses;
	const Etoile::Joints& joints = skeleton->getJoints();
	Etoile::Joints::const_iterator itor = joints.begin();
	for(;itor != joints.end(); ++itor)
	{
		Joint* j = *itor;
		poses.push_back(j->getWorldPosition());
	}
	_boundingBox.build(poses);
	_scaleFactor = (_boundingBox.maximum() - _boundingBox.minimum()).length() / 10.0;
}

void MocapWidget::setPaused()
{
	_pause = true;
	emit paused();
}

unsigned int MocapWidget::getFPS()
{
	return _fps;
}

void MocapWidget::setFPS(int fps)
{
	_timer.stop();
	int period = 1000.0 / float(fps);
	_timer.start(period);
}

void MocapWidget::updateFrameByTimer()
{
	updateFrame();
}

void MocapWidget::updateFrame(int idx)
{
	KeyFrameSequence* frames = _bvhloader.getKeyFrameSequence();
	Skeleton* skeleton = _bvhloader.getSkeleton();
	if(idx < 0)
	{
		if(_pause) return;
		_frameIdx = _frameIdx++;
		if(_frameIdx >= frames->getKeyFrames().size() && _cycle == true)
		{
			_frameIdx = 0;
		}
		else if(_frameIdx >= frames->getKeyFrames().size() && _cycle == false)
		{
			_frameIdx = 0;
			setPaused();
		}
		emit frameChanged(_frameIdx);
	}
	else
	{
		_frameIdx = idx;
	}
	updateSkeleton(skeleton);
}

void MocapWidget::updateSkeleton(Skeleton* skeleton)
{
	KeyFrameSequence* frames = _bvhloader.getKeyFrameSequence();
	if(_frameIdx >= frames->getKeyFrames().size() || _frameIdx < 0)
	{
		return;
	}
	KeyFrame* f = frames->getKeyFrames()[_frameIdx];
	std::map<std::string, int>& indices = f->getIndices();
	std::vector<FrameParameters>& infos = f->getFrameParameters();
	std::map<std::string, int>::iterator itor = indices.begin();
	for(; itor != indices.end(); ++itor)
	{
		std::string name = itor->first;
		int idx = itor->second;
		Joint* j = skeleton->getJoint(name);
		j->setLocalRotation(infos[idx]._localRotation);
		j->update();
	}
	skeleton->update();
}

void MocapWidget::drawSkeleton(Skeleton* skeleton)
{
	//updateFrame();
	{
		const Etoile::Joints& joints = skeleton->getJoints();
		Etoile::Joints::const_iterator itor = joints.begin();
		for(;itor != joints.end(); ++itor)
		{
			Etoile::Joint* joint = (*itor);
			//joint->updateLocally();
			//joint->draw();
			//joint->drawAxis();
			//joint->drawUpVector();
			const Vec3f& pos2 = joint->getWorldPosition();
			float DiffuseMaterial[3] = {0.5, 0.0, 0.0};          // define bones color
			glColor3fv(DiffuseMaterial);
			glPointSize(5);
			glBegin(GL_POINTS);
			glVertex3d(pos2.x()/_scaleFactor, pos2.y()/_scaleFactor, pos2.z()/_scaleFactor);
			glEnd();
			Joint* jointparent = joint->getParent();
			if(jointparent !=0)
			{
				const Vec3f& pos1 = jointparent->getWorldPosition();
				float DiffuseMaterial[3] = {0.3, 0.3, 0.6};          // define bones color
				glColor3fv(DiffuseMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, &DiffuseMaterial[0]);
				glDrawFunctions drawer;
				drawer.drawBone(pos1/_scaleFactor, pos2/_scaleFactor, joint->getLocalPosition(), jointparent->getWorldRotation());
			}

		}
	}

}

void MocapWidget::drawInfo(Joint* currentJoint)
{
	if(currentJoint == NULL) return;

	const Vec3f& pos2 = currentJoint->getWorldPosition();
	{
		if(currentJoint->getParent() != NULL)
		{
			drawCurrentAxis(pos2.x()/_scaleFactor, pos2.y()/_scaleFactor, pos2.z()/_scaleFactor, currentJoint->getParent()->getWorldRotation());
			//drawUpDirectionalVector(pos2.x(), pos2.y(), pos2.z(), currentJoint->getUpDirectionVector());
		}
		else
			drawCurrentAxis(pos2.x()/_scaleFactor, pos2.y()/_scaleFactor, pos2.z()/_scaleFactor, Quaternionf());
	}


	glLineWidth(0.3);
	Vec3f pos = currentJoint->getWorldPosition();
	glColor3f(1,0.4,0.7);
	qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(qglviewer::Vec(pos.x()/_scaleFactor, pos.y()/_scaleFactor, pos.z()/_scaleFactor));
	drawText((int)screenPos[0]+20, (int)screenPos[1]-10, QString(" %4: [ %1, %2, %3 ] ").arg(pos.x(), 0, 'f',2).arg(pos.y(), 0, 'f',2).arg(pos.z(), 0, 'f',2).arg(QString(currentJoint->getName().c_str())) );

}


void MocapWidget::draw()
{
	GLfloat light_position0[] = { 0, 30, 50, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	GLfloat light_position1[] = { 0, 30, -50, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	printOpenGLError();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	printOpenGLError();

	drawPlane();
	if(_bvhloader.getSkeleton() != NULL)
	{
		drawSkeleton(_bvhloader.getSkeleton());
		if(_selectedJointIndex > 0 && _selectedJointIndex < _bvhloader.getSkeleton()->getJoints().size())
		{
			drawInfo(_bvhloader.getSkeleton()->getJoint(_selectedJointIndex));
		}
	}

	printOpenGLError();
	//drawAxis();
	glColor3f(0.9,0.25,0.55);
	QFont serifFont("Times", 10, QFont::Bold);
	drawText((int)30, (int)25, QString(" shift + right_mouse : choose one joint !"), serifFont);

}


void MocapWidget::drawPlane()
{
	glColor3f(0.8,0.8,0.8);
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3d(0,1,0);
	glVertex3f(5000,(_boundingBox.minimum().y() - 0.1) / _scaleFactor,50000);
	glVertex3f(5000,(_boundingBox.minimum().y() - 0.1) / _scaleFactor,-50000);
	glVertex3f(-5000,(_boundingBox.minimum().y() - 0.1) / _scaleFactor,-50000);
	glVertex3f(-5000,(_boundingBox.minimum().y() - 0.1) / _scaleFactor,50000);
	glEnd();
	glPopMatrix();

}

void MocapWidget::releaseResource()
{

}



void MocapWidget::drawWireFrame()
{

}


void MocapWidget::keyPressEvent(QKeyEvent *e)
{


	if (e->key() == Qt::Key_F5)
	{
		openBVH();
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
		//initFramework();
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


void MocapWidget::mouseMoveEvent(QMouseEvent* event)
{
	if ( (event->modifiers() & Qt::ShiftModifier) )
	{


	}
	else
		QGLViewer::mouseMoveEvent(event);
}

void MocapWidget::mousePressEvent(QMouseEvent* event)
{
	if ((event->button() == Qt::RightButton)&&(event->modifiers() == Qt::ControlModifier))
	{


	}
	else
		if ((event->modifiers() & Qt::ShiftModifier))
		{
			if(_bvhloader.getSkeleton() != NULL)
			{
				// select one mass
				qglviewer::Camera *cam = this->camera();
				int x = event->x();
				int y = event->y();
				float minScreenDistance = 10000;

				int i = 0;
				std::string name;
				const Etoile::Joints& joints = _bvhloader.getSkeleton()->getJoints();
				Etoile::Joints::const_iterator itor = joints.begin();
				for(;itor != joints.end(); ++itor, ++i)
				{
					Joint* joint = (*itor);
					Vec3f pos = joint->getWorldPosition();
					qglviewer::Vec screenPos = cam->projectedCoordinatesOf(qglviewer::Vec(pos[0]/_scaleFactor,pos[1]/_scaleFactor,pos[2]/_scaleFactor));
					float dis = Vec2f(   Vec2f(screenPos[0] , screenPos[1]) - Vec2f(x , y)   ).length();
					if( dis < minScreenDistance )
					{
						minScreenDistance = dis;
						_selectedJointIndex = i;
						name = joint->getName();
					}

				}

				std::cout<<"selected Joint: " << name <<std::endl;
			}

		}
		else 
			if ((event->modifiers() & Qt::ControlModifier)&& (event->button() == Qt::LeftButton))
			{


			}
			else if((event->modifiers() & Qt::ControlModifier))
			{

			}
			else
			{
				QGLViewer::mousePressEvent(event);


			}
}

void MocapWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QGLViewer::mouseReleaseEvent(event);
}

void MocapWidget::wheelEvent ( QWheelEvent * event ) 
{
	if (event->modifiers() & Qt::ShiftModifier)
	{

	}
	else
		QGLViewer::wheelEvent(event);
}


std::string translate(std::string name){
	if(name == "Hips")
		return "HumanoidRoot";
	else if(name == "LeftUpLeg"){
		return "l_hip";
	}else if(name == "LeftLeg"){
		return "l_knee";
	}else if(name == "LeftFoot"){
		return "l_ankle";
	}else if(name == "LeftFootHeel"){
		return "l_metatarsal";
	}else if(name == "RightUpLeg"){
		return "r_hip";
	}else if(name == "RightLeg"){
		return "r_knee";
	}else if(name == "RightFoot"){
		return "r_ankle";
	}else if(name == "RightFootHeel"){
		return "r_metatarsal";
	}else if(name == "Spine"){
		return "vl5";
	}else if(name == "Spine1"){
		return "vt7";
	}else if(name == "LeftShoulder"){
		return "l_acromioclavicular";
	}else if(name == "LeftArm"){
		return "l_shoulder";
	}else if(name == "LeftForeArm"){
		return "l_elbow";
	}else if(name == "LeftHand"){
		return "l_wrist";
	}else if(name == "RightShoulder"){
		return "r_acromioclavicular";
	}else if(name == "RightArm"){
		return "r_shoulder";
	}else if(name == "RightForeArm"){
		return "r_elbow";
	}else if(name == "RightHand"){
		return "r_wrist";
	}else if(name == "Neck"){
		return "vc1";
	}else if(name == "Head"){
		return "skullbase";
	}else
		return name;

}

void MocapWidget::drawUpDirectionalVector(float x, float y, float z, Vec3f v)
{
	glPushMatrix();

	float length = 5;
	glLineWidth(10);
	glTranslatef(x,y,z);
	glPushMatrix();
	glColor4f(1,1,0,1);
	glBegin(GL_LINES);
	glVertex3f(v.x() * length , v.y() * length , v.z() * length);
	glVertex3f(0,0,0);
	glEnd();

	glPopMatrix();
	//drawAxis(10);
	glPopMatrix();
}

void MocapWidget::drawCurrentAxis(float x, float y, float z, Quaternionf q)
{
	glPushMatrix();
	float length = 10.0 /_scaleFactor;
	Vec3f xAxis(1,0,0);
	Vec3f yAxis(0,1,0);
	Vec3f zAxis(0,0,1);
	glLineWidth(3);
	// Vec3f angle = q.getEulerAngleXYZBYAngle();
	glTranslatef(x,y,z);
	xAxis = q * xAxis;
	yAxis = q * yAxis;
	zAxis = q * zAxis;
	glPushMatrix();
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
	glPopMatrix();

	//    glRotatef(angle.x(), 1, 0, 0);
	//    glRotatef(angle.y(), 0, 1, 0);
	//    glRotatef(angle.z(), 0, 0, 1);

	//    drawAxis(10);
	glPopMatrix();
}


//#include <iostream>
//#include <fstream>
//void MocapWidget::outputBap(BVHFrames& frames)
//{
//	std::vector<BVHFrame>& fs  = frames.getFrames();
//	std::string output="";
//	for(unsigned int i = 0; i < fs.size(); ++i){
//		BVHFrame& f = fs[i];
//		JointMotions& jms = f.getJointMotions();
//		BAPFrame bf(i);
//		for(unsigned j = 0; j < jms.size(); ++j)
//		{
//			JointMotion& jm = jms[j];
//			std::string name = jm._jointName;
//			Quaternionf rotation;
//			Vec3f position;
//			for(unsigned int k = 0; k < jm._channels.size(); ++k)
//			{
//				Channel& ch = jm._channels[k];
//				ChannelType type = ch._type;
//				float rotationValue = ch._value / 180.0 * 3.14159265;
//
//				if (type == X_POSITION)
//				{
//					position[0] = ch._value;
//				}
//				else if (type == Y_POSITION)
//				{
//					position[1] = ch._value;
//				}
//				else if (type == Z_POSITION)
//				{
//					position[2] = ch._value;
//				}
//				else if (type == X_ROTATION)
//				{
//					Quaternionf q;
//					q.setAxisAngle(Vec3f(1,0,0), rotationValue);
//					//rotation =  q * rotation;
//					rotation =  rotation * q;
//				}
//				else if (type == Y_ROTATION)
//				{
//					Quaternionf q;
//					q.setAxisAngle(Vec3f(0,1,0), rotationValue);
//					//rotation =  q * rotation;
//					rotation =  rotation * q;
//
//				}
//				else if (type == Z_ROTATION)
//				{
//					Quaternionf q;
//					q.setAxisAngle(Vec3f(0,0,1), rotationValue);
//					//rotation =  q * rotation;
//					rotation =  rotation * q;
//				}
//			}
//
//			Vec3f angle = rotation.getEulerAngleXYZ();
//
//
//			BAPType z = BAPConverter::MJointToBAP(translate(name) + ".rotateZ");
//			BAPType y = BAPConverter::MJointToBAP(translate(name) + ".rotateY");
//			BAPType x = BAPConverter::MJointToBAP(translate(name) + ".rotateX");
//			bf.SetBAP(z, angle.z() * 100000);
//			bf.SetBAP(y, angle.y() * 100000);
//			bf.SetBAP(x, angle.x() * 100000);
//
//
//		}
//		std::string bap = bf.WriteBAP();
//		//printf( bap.c_str());
//		output.append(bap);
//	}
//
//	//printf( output.c_str());
//
//	ofstream myfile;
//	myfile.open ("example.bap");
//	myfile <<"0.0 xxx 25 1"<<std::endl;
//	myfile << output;
//	myfile.close();
//}
