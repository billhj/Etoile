#ifndef DYNAMICSSKELETONDEMO_H
#define DYNAMICSSKELETONDEMO_H

#include <QtGui/QMainWindow>
#include "ui_DynamicsSkeletonDemo.h"
#include "DynamicsGLWidget.h"

class DynamicsSkeletonDemo : public QMainWindow
{
	Q_OBJECT

public:
	DynamicsSkeletonDemo(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DynamicsSkeletonDemo();

private:
	Ui::DynamicsSkeletonDemoClass ui;
	DynamicsGLWidget* _widget;
};

#endif // DYNAMICSSKELETONDEMO_H
