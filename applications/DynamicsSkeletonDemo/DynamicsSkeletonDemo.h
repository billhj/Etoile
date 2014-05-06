#ifndef DYNAMICSSKELETONDEMO_H
#define DYNAMICSSKELETONDEMO_H

#include <QtGui/QMainWindow>
#include <QDockWidget>
#include "ui_DynamicsSkeletonDemo.h"
#include "DynamicsGLWidget.h"

class DynamicsSkeletonDemo : public QMainWindow
{
	Q_OBJECT

public:
	DynamicsSkeletonDemo(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DynamicsSkeletonDemo();
	public slots:
		void changeDeltaT(double t);
private:
	Ui::DynamicsSkeletonDemoClass ui;
	DynamicsGLWidget* _widget;
	QDockWidget* _dock;
};

#endif // DYNAMICSSKELETONDEMO_H
