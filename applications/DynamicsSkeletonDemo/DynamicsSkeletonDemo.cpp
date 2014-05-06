#include "DynamicsSkeletonDemo.h"

DynamicsSkeletonDemo::DynamicsSkeletonDemo(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_widget = new DynamicsGLWidget(this);
	this->setCentralWidget(_widget);
}

DynamicsSkeletonDemo::~DynamicsSkeletonDemo()
{

}
