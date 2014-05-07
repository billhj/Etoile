#include "DynamicsSkeletonDemo.h"
#include <QFormLayout>

DynamicsSkeletonDemo::DynamicsSkeletonDemo(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_widget = new DynamicsGLWidget(this);
	this->setCentralWidget(_widget);
	_dock_pd = new QDockWidget(this);
	this->addDockWidget(Qt::RightDockWidgetArea, _dock_pd);
	QWidget* widget = new QWidget();
	_dock_pd->setWidget(widget);

	QFormLayout* layout = new QFormLayout();
	QDoubleSpinBox* dt = new QDoubleSpinBox();
	dt->setRange(0.01, 0.1);
	dt->setSingleStep(0.001);
	dt->setDecimals(3);
	layout->addRow("dt", dt);
	kp = new QDoubleSpinBox();
	kp->setRange(0.01, 3000);
	kp->setValue(1000);
	kp->setSingleStep(0.1);
	layout->addRow("kp", kp);
	kd = new QDoubleSpinBox();
	kd->setRange(0.01, 3000);
	kd->setSingleStep(0.1);
	kd->setValue(10);
	layout->addRow("kd", kd);
	
	widget->setLayout(layout);
	
	connect(dt, SIGNAL(valueChanged(double)), this, SLOT(changeDeltaT(double)));
	connect(_widget, SIGNAL(jointClicked()), this, SLOT(jointSelected()));
	connect(kp, SIGNAL(valueChanged(double)), this, SLOT(setKp(double)));
	connect(kd, SIGNAL(valueChanged(double)), this, SLOT(setKd(double)));
}

void DynamicsSkeletonDemo::setKp(double)
{
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];
			if(_widget->_usePD)
			{
				((Etoile::PDJointMotor*)motor)->setKp(kp->value());
			}
		}
	}
}

void DynamicsSkeletonDemo::setKd(double)
{
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];
			if(_widget->_usePD)
			{
				((Etoile::PDJointMotor*)motor)->setKd(kd->value());
			}
		}
	}
}

void DynamicsSkeletonDemo::changeDeltaT(double t)
{
	_widget->dt = t;
}

void DynamicsSkeletonDemo::jointSelected()
{
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];
			if(_widget->_usePD)
			{
				kd->setValue(((Etoile::PDJointMotor*)motor)->Kd());
				kp->setValue(((Etoile::PDJointMotor*)motor)->Kp());
			}
		}
	}
}

DynamicsSkeletonDemo::~DynamicsSkeletonDemo()
{

}
