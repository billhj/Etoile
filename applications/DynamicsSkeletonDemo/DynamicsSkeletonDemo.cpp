#include "DynamicsSkeletonDemo.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
DynamicsSkeletonDemo::DynamicsSkeletonDemo(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_widget = new DynamicsGLWidget(this);
	this->setCentralWidget(_widget);
	_dock = new QDockWidget(this);
	this->addDockWidget(Qt::RightDockWidgetArea, _dock);
	QWidget* widget = new QWidget();
	_dock->setWidget(widget);

	QFormLayout* layout = new QFormLayout();
	QDoubleSpinBox* dt = new QDoubleSpinBox();
	dt->setRange(0.01, 0.035);
	dt->setSingleStep(0.001);
	dt->setDecimals(3);
	layout->addRow("dt", dt);
	widget->setLayout(layout);
	
	connect(dt, SIGNAL(valueChanged(double)), this, SLOT(changeDeltaT(double)));

}


void DynamicsSkeletonDemo::changeDeltaT(double t)
{
	_widget->dt = t;
}

DynamicsSkeletonDemo::~DynamicsSkeletonDemo()
{

}
