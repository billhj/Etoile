#ifndef DYNAMICSSKELETONDEMO_H
#define DYNAMICSSKELETONDEMO_H

#include <QtGui/QMainWindow>
#include <QDockWidget>
#include "ui_DynamicsSkeletonDemo.h"
#include "DynamicsGLWidget.h"
#include <QDoubleSpinBox>

class DynamicsSkeletonDemo : public QMainWindow
{
	Q_OBJECT

public:
	DynamicsSkeletonDemo(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DynamicsSkeletonDemo();
	public slots:
		void changeDeltaT(double t);
		void jointSelected();
		void setKp(double);
		void setKd(double);
private:
	Ui::DynamicsSkeletonDemoClass ui;
	DynamicsGLWidget* _widget;
	QDockWidget* _dock_pd;
	QDoubleSpinBox* kp;
	QDoubleSpinBox* kd;
};

#endif // DYNAMICSSKELETONDEMO_H
