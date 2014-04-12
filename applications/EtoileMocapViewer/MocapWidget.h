/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MocapWidget.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/glhead.h"
#include <QGLViewer/qglviewer.h>
#include <QTime>
#include <QMainWindow>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QSize>
#include <QTimer>
#include "animation/BVHLoader.h"
#include "geometry/AxisAlignedBoundingBox.h"

using namespace Etoile;

class MocapWidget : public QGLViewer
{
	Q_OBJECT
		
	static const int WIDGETWIDTH = 800, WIDGETHEIGHT = 600;

public:
	MocapWidget(QMainWindow* parent = 0);
	~MocapWidget();

	void initFramework(const std::string&);
	void drawPlane();
	void drawWireFrame();
	void drawCurrentAxis(float x, float y, float z, Quaternionf q);
	void drawUpDirectionalVector(float x, float y, float z, Vec3f v);
	void releaseResource();
	void openBVH();
	void updateFrame(int idx = -1);
	void setPaused();
	unsigned int getFPS();
	void setFPS(int fps);
protected:
	void keyPressEvent(QKeyEvent *e);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent ( QWheelEvent * event );

private:
	virtual void draw();
	void drawSkeleton(Skeleton* skeleton);
	void updateSkeleton(Skeleton* skeleton);
	void drawInfo(Joint* currentJoint);

	virtual void init();
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

	// QSize minimumSizeHint () const {return QSize(1024,768);}

	//void outputBap(BVHFrames& frames);


	QMainWindow* _pParent;

signals:
	void frameChanged(int);
	void paused();
public slots:
	void updateFrameByTimer();
public:

	int _selectedJointIndex;
	int _maxnb_iterations;
	bool _useConstraints;
	BVHLoader _bvhloader;
	int _frameIdx;

	bool _pause;
	bool _cycle;
	unsigned int _fps;
	QTimer _timer;

	AxisAlignedBoundingBoxf _boundingBox;
	float _scaleFactor;
};
